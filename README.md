# Drummock

A fast and simple drum sampler meant for trap/hip-hip production. Built with c++ and JUCE.

## Installation
Download the appropriate binary from [releases](https://github.com/ameyakakade/drummock/releases) and move it to your plugin folder

### MacOS
Plugins are usually in `/Library/Audio/Plug-Ins/`
Move the `au` and `vst3` in the appropriate folders.
If daw says "Drummock cannot be opened" this is because the MacOS binaries are not notarized.
To fix run the following commands in the terminal:
```bash
# For VST3
sudo xattr -cr /Library/Audio/Plug-Ins/VST3/Drummock.vst3

# For AU
sudo xattr -cr /Library/Audio/Plug-Ins/Components/Drummock.component
```

### Windows
Move the `vst3` into `C:\Program Files\Common Files\VST3`

### Linux
Move the `vst3` into `~/.vst3` or `/usr/lib/vst3`
