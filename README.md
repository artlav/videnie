Videnie is a trajectory drawing module for Orbiter Space Flight Simulator( http://orbit.medphys.ucl.ac.uk/ ).  
It renders vessel and planet trajectories straight into the main screen.  
Compatible with internal D3D7 graphics only, as it was hooking up to them.

Full add-on files are at: https://www.orbithangar.com/showAddon.php?id=5f133ad0-a3c1-4de8-a455-ad7a3604d249  
Depends on KOST by C J Plooy: https://www.orbithangar.com/showAddon.php?id=f01277c9-6625-4a17-bff8-3e1cfa654004

Dump of the code as of 110912 release, provided as it was found in the backups.  
I no longer have the means to check if it still works or compiles.  
Can't remember where grhook.dll came from, all it did was hooking up to D3D render calls to enable the overlay.

## Original add-on manual:

## Installation:
Unpack into Orbiter directory, overwriting existing version (if any), activate videnie module.  
If an error messae box pops up on module activation, check installation.

## Usage:
Alt+I is the master key to call up the controls, follow it with keys  
0 toggles all trajectories on and off.  
1 toggles focused vessel trajectory  
2 toggles all vessels trajectories  
3 toggles planet trajectories  
4 hide lines in internal view  
5 hide landed vessels  
6 hide lines behind planets  
7 panning mode  
8 hide tiny worlds  
9 toggle panning mode key (wheel or left button)

In panning mode hold middle mouse button (or left one if 9 is active) to pan around.  
Only works in external view.  
Saving with it on is not recommended.

Tiny world is defined by radius, cut-off is "tiny_cutoff" parameter in the cfg file.

"precision" parameter in cfg file defines trajectory precision.  
The higher the nicer, but less FPS, the lower the uglier, but more FPS.

exclude_object line in cfg file can be used to explicitly exclude an object from drawing.

## Limitations:
- Only two-body elements are considered
- Close-up the lines might look garbled

## Changes:

#### 110912:
- Fixed misalignment in unprojection (labels correspond to objects now)
- Added panning toggle

#### 101207:
- Exclude list option (Deimos is off by default)
- Possible fixes to some reported CTDs
- Possible fix for DEP issue
- Possible fix to panning CTDs
- Message about panning mode being active
- Message about shutdown mode problems
- No annoying messages option

#### 101005:
- Fixed panning reinit and crashes
- Fixed crash on restore with panning on.
- Fixed panning discontinuities
- Fixed panning reset on re-init
- Planets and moons drawn in different colors
- Option to ignore worlds smaller than the tiny_cutoff parameter in cfg file
- Hide landed option now include focused vessel
- A few random fixes

#### 101004-2:
- Panning mode

#### 101004:
- Near clip distance: target larger than tenth of the screen height
- Far clip distance: target trajectory takes less then 20 pixels on screen
- Fixed orbits for all moons
- Added orbits for planets
- Changed toggle logic: Alt+I is the master key, followed by 0 thru 9 as a command. So Alt+I, 0 toggles all.
- No message shown unless toggled
- Toggle for internal view lines drawing
- Toggle for not showing landed vessels
- Toggle for not drawing lines behind planets
- Fixed hyperbolic lines
- Cfg file for all of that
