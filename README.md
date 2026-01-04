# Drum Sampler (WIP)

Work-in-progress JUCE-based drum sampler focused on making sample loading fast and simple.

This repository is currently under active development.

## Building
### MacOS
Plugin is built using xcode. Make sure you have Xcode command line tools installed.

This repo uses JUCE as a submodule.
Use `git clone --recurse-submodules https://github.com/ameyakakade/drumsampler` while cloning or `git submodule update --init --recursive` after cloning.

Run `cmake -G Xcode build` to generate a Xcode project. 
To build, run `cmake --build build` or compile in Xcode
