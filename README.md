# LiftoffSimulator
Mods and utilities for using the Liftoff flight sim as a simulator for testing flight control algorithms

The base program contains a test script to read all data out of the Liftoff Simulator.

- `lf_telemetry` - Pulls base data out of the liftoff simulator
- `joystick` - Creates a virtual joystick to send commands to liftoff

## Getting Basic Telemetry
Liftoff can export a lot of data over UDP without any modifications.
See this [post on how to](https://steamcommunity.com/sharedfiles/filedetails/?id=3160488434).

Place the following snippet in `~/.config/unity3d/LuGus Studios/Liftoff/TelemetryConfiguration.json`
```json
{ 
	"EndPoint": "127.0.0.1:9001", 
	"StreamFormat": [ 
		"Timestamp", 
		"Position", 
		"Attitude", 
		"Velocity", 
		"Gyro", 
		"Input", 
		"Battery", 
		"MotorRPM" 
	] 
}
```


## UnoController
Sample Arduino program that can "act" as the control board

## LidarMod
A Unity mod that can be injected into the Liftoff game to forward LiDAR data over a UDP port.

### Building the Mod
To build the mod, install liftoff and install Microsoft DotNet.
Then, ensure that the `LiftoffLidar.csproj` file contains the correct `HintPath` references.
By default, the project expects the game data to live under `/home/jacob/.local/share/Steam/steamapps/common/Liftoff`.
Ensure that absolute paths are used.
Microsoft DotNet may struggle to build the project when relative paths are used.

After ensuring the project is setup, build the mod with:
```
dotnet build
```

### Installing BepInEx
BepInEx is required to load the mod.
It can be downloaded from [https://github.com/bepinex/bepinex/releases](https://github.com/bepinex/bepinex/releases).
Extract the contents of the mod loader into the Liftoff base folder to install it.

#### Configuring BepInEx
Before the mod can be loaded, the BepInEx configuration must be updated.
The configuration file is located under `BepInEx/config/BepInEx.cfg`.

First, disable caching:
```cfg
[Caching]

## Enable/disable assembly metadata cache
## Enabling this will speed up discovery of plugins and patchers by caching the metadata of all types BepInEx discovers.
# Setting type: Boolean
# Default value: true
EnableAssemblyCache = false
```

Then, ensure that the manager object is hidden from Unity:
```cfg
[Chainloader]

## If enabled, hides BepInEx Manager GameObject from Unity.
## This can fix loading issues in some games that attempt to prevent BepInEx from being loaded.
## Use this only if you know what this option means, as it can affect functionality of some older plugins.
## 
# Setting type: Boolean
# Default value: false
HideManagerGameObject = true
```

Finally, set logging to all:
```cfg
[Harmony.Logger]

## Specifies which Harmony log channels to listen to.
## NOTE: IL channel dumps the whole patch methods, use only when needed!
# Setting type: LogChannel
# Default value: Warn, Error
# Acceptable values: None, Info, IL, Warn, Error, Debug, All
# Multiple values can be set at the same time by separating them with , (e.g. Debug, Warning)
LogChannels = All
```
### Installing the Mod
Copy the mod from the build folder (`LidarMod/LiftoffLidar/bin/Debug/netstandard2.1/LiftoffLidar.dll`)
to the plugins folder within `BepInEx/plugins`.


