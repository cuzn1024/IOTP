# IOT-Platform #

## Directories ##
- deps	             (depends)
- doc	             (documentation)
- gen	             (generations)
- project            (project files)
- src	             (.h and .cpp files)
- tools              (other tools)
- IOT-Platform.sln	(solution file)

## First Build
Create deps/sqlite/include, deps/sqlite/lib/win32 and deps/sqlite/lib/x64.

## Depends ##
1. boost	(log system and more)
2. sqlite   (repository for data)

## hierachy ##

- plugin	(all plugins)
	- transceiver
		- socket	(network interface)
- main	(main project)

## Others ##
1. The solution does not support Release build for Windows XP 64bit.
2. Currently the target does not support path in Chinese characters. Maybe someday later I will fixup it;