using BepInEx;
using BepInEx.Logging;
using UnityEngine;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;


namespace LiftoffLidarMod
{
    [BepInPlugin("com.jacob.lidarmod", "Liftoff Lidar", "1.0.0")]
    public class Plugin : BaseUnityPlugin
    {
        internal static ManualLogSource Log;
        private UdpClient udpClient;
        private IPEndPoint remoteEndPoint;
        int updateCounter = 0;
        private const int N_POINTS = 180;

        // Awake runs once when the mod is loaded
        private void Awake()
        {
            Log = BepInEx.Logging.Logger.CreateLogSource("LidarMod");
            Log.LogInfo("Lidar Mod starting up");
            try{
                udpClient = new UdpClient();
                remoteEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 9002);
                Log.LogInfo("UDP Client Ready on port 9002");
            }
            catch (Exception e){
                Log.LogError($"Failed to setup UDP: {e.Message}");
            }
            Logger.LogInfo("Lidar Mod Loaded");
            // string message = "Hello\n";
            // byte[] sendBytes = Encoding.ASCII.GetBytes(message);
            // udpClient.Send(sendBytes, sendBytes.Length, remoteEndPoint);
        }


        private void FixedUpdate(){
            // string message = $"FixedUpdate {updateCounter}\n";
            // byte[] sendBytes = Encoding.ASCII.GetBytes(message);
            // udpClient.Send(sendBytes, sendBytes.Length, remoteEndPoint);
            updateCounter = updateCounter + 1;

            GameObject drone = GameObject.Find("DroneHUD_Custom(Clone)");
            if (drone == null){
                if(updateCounter % 100 == 0){
                    Log.LogInfo("LidarMod: Player not found..");
                }
                return;
            }
            else {
                if(updateCounter % 100 == 0){
                    Log.LogInfo($"LidarMod: Drone found at {drone.transform.position}");
                }
            }

            // Slow down the following loop
            if(updateCounter % 3 != 0){
                return;
            }

            float[] distances = new float[N_POINTS*2];
            Vector3 origin = drone.transform.position;

            for (int i = 0; i < N_POINTS; i++)
            {
                // Scan forward and to the sides
                Vector3 direction = Quaternion.AngleAxis(i*(360f/N_POINTS), drone.transform.up) * drone.transform.forward;
                if (Physics.Raycast(origin, direction, out RaycastHit hit, 50f))
                {
                    distances[i] = hit.distance;
                }
                else
                {
                    distances[i] = 50.0f;
                }

                // Scan up and to the sides
                direction = Quaternion.AngleAxis(i*(360f/N_POINTS), drone.transform.forward) * drone.transform.up;
                if (Physics.Raycast(origin, direction, out hit, 50f))
                {
                    distances[N_POINTS+i] = hit.distance;
                }
                else
                {
                    distances[N_POINTS+i] = 50.0f;
                }
            }

            byte[] payload = new byte[distances.Length * 4];
            Buffer.BlockCopy(distances, 0, payload, 0, payload.Length);
            udpClient.Send(payload, payload.Length, remoteEndPoint);
            // string message = $"FixedUpdate {updateCounter}: {drone.transform.position} - {distances[0]}\n";
            // byte[] sendBytes = Encoding.ASCII.GetBytes(message);
            // udpClient.Send(sendBytes, sendBytes.Length, remoteEndPoint);
        }
    }
}
