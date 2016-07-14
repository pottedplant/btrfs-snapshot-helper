# btrfs-snapshot-helper

FOR EDUCATIONAL PURPOSES ONLY. NO WARRANTY.

## Build

Requirements:
* c++11 compiler
* boost
* boost-build

```
b2 -j8 release link=static
```

> link=static is not strictly needed and requires static version of boost libraries to be installed

## Usage

```
> btrfs-snapshot-helper -h
program options:
  -h [ --help ]                      show help
  -v [ --subvolume ] path            source subvolume to take snapshot of
  -d [ --snapshots-dir ] path        snapshot storage directory
  -s [ --strategy ] name             now,once-per-day
  -m [ --max-snapshots ] number (=0) use 0 for unlimited
  -l [ --log-level ] level (=warn)
```
