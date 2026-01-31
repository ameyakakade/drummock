# Drummock
A fast and simple drum sampler meant for trap/hip-hip production. Built with c++ and JUCE.

<summary><h2>Screenshot</h2></summary>
<img width="963" height="822" alt="Screenshot 2026-01-31 at 7 00 45 PM" src="https://github.com/user-attachments/assets/2b318039-139f-41d3-8af0-1d4cacd905d8" />



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

## Features
- Easy file loading, just drag and drop! Drag in multiple files to load them at the same time.
- Polyphony support.
- Linear interpolation for resampling and pitch shifting for that mpc sound.
- Built in hard clipper
- Randomizing for volume, pan and pitch for realistic movement.

## Limitations
- Voice stealing doesn't have a fade out.
- Absolute paths are stored, plugin will not find the sample if they are moved.
