# GoBoardController — Codex Development Rules

We are building a JUCE + CMake audio plugin called GoBoardController.

This project must use a modern CMake workflow.
Do not use Projucer.
Do not create a `.jucer` file.
Do not create `JuceLibraryCode`.
Do not create `Builds/MacOSX`.
CMake is the single source of truth.

## Primary architecture reference

Use `sudara/pamplejuce` as the primary architecture and build-system reference.

Reference repo:
`sudara/pamplejuce`

Follow its general approach:

* JUCE as a submodule.
* Modern CMake.
* `juce_add_plugin(...)`.
* Shared plugin code target when useful.
* Testable non-GUI logic.
* Melatonin Inspector for GUI debugging.
* Clean separation between build setup, source code, tests, and packaging.

Do not blindly copy the whole Pamplejuce project.
Keep our first version smaller than Pamplejuce if possible.

The project name should be:
`GoBoardController`

The product name should be:
`Go Board Controller`

Plugin formats:

* Standalone
* VST3

Optional later:

* AU
* CLAP

## Functional reference

Use `madskjeldgaard/Birdhouse` only as a functional reference for OSC/MIDI control routing.

Reference repo:
`madskjeldgaard/Birdhouse`

Study it for:

* OSC and MIDI bridge concepts.
* Multiple control streams.
* Range scaling.
* MIDI CC output design.
* Visual feedback for control streams.

Do not copy Birdhouse architecture directly.
Do not convert our plugin into an OSC receiver.
Our plugin generates control data internally from the Go board.

Birdhouse direction:
OSC input -> MIDI output

Our direction:
Go board state -> 8 macro values -> OSC output and MIDI CC output

## Legacy conceptual reference

`thomasgeissl/MIDI2OSC` may be used only as a conceptual reference for a JUCE plugin that sends OSC and does not generate audio.

Do not use it as a build-system reference.
Do not copy its project structure.
Do not create a `.jucer` project.
Do not create `JuceLibraryCode`.

## Current plugin purpose

This is not a synthesizer yet.
This is not an AI plugin yet.
This is not a hardware or FPGA project yet.

The first version is a musical controller plugin.

Core signal flow:

9x9 Go board
-> board state matrix
-> rule-based mapping engine
-> 8 macro values
-> GUI display
-> OSC output
-> MIDI CC output

Audio should pass through unchanged or remain silent depending on the plugin configuration.
Do not implement a synthesizer engine in the first milestone.

## Required source modules

Create and maintain these core classes:

1. `BoardState`
   Files:

* `source/BoardState.h`
* `source/BoardState.cpp`

Responsibilities:

* Store a 9x9 board.
* Each cell can be Empty, Black, or White.
* Use fixed-size storage, preferably `std::array`.
* No JUCE GUI code.
* No OSC code.
* No MIDI code.

2. `MappingEngine`
   Files:

* `source/MappingEngine.h`
* `source/MappingEngine.cpp`

Responsibilities:

* Convert `BoardState` into 8 normalized macro values.
* Output type: `std::array<float, 8>`.
* Values must stay in range 0.0f to 1.0f.
* No GUI code.
* No OSC code.
* No MIDI code.
* No neural network code.

Initial macro definitions:

1. Density

2. Black/White Balance

3. Center X

4. Center Y

5. Edge Ratio

6. Conflict

7. Symmetry

8. Cluster

9. `BoardComponent`
   Files:

* `source/BoardComponent.h`
* `source/BoardComponent.cpp`

Responsibilities:

* Draw the 9x9 Go board.
* Draw black and white stones.
* Handle mouse interaction.
* Left click cycles Empty -> Black -> White -> Empty.
* Right click clears a cell.
* Use Melatonin Inspector support for GUI layout/debugging if available.
* Do not calculate macro values here.

4. `OutputRouter`
   Files:

* `source/OutputRouter.h`
* `source/OutputRouter.cpp`

Responsibilities:

* Manage OSC sending with JUCE OSC classes.
* Prepare MIDI CC output logic.
* Default OSC host: `127.0.0.1`
* Default OSC port: `9000`
* OSC addresses:

  * `/goboard/macro/1`
  * `/goboard/macro/2`
  * `/goboard/macro/3`
  * `/goboard/macro/4`
  * `/goboard/macro/5`
  * `/goboard/macro/6`
  * `/goboard/macro/7`
  * `/goboard/macro/8`

Do not send OSC from the audio thread.
Do not perform network operations directly inside `processBlock`.

5. `PluginProcessor`

Responsibilities:

* Keep JUCE audio-thread rules.
* Keep `processBlock` real-time safe.
* Do not allocate in `processBlock`.
* Do not perform file I/O in `processBlock`.
* Do not perform OSC network sending in `processBlock`.
* Support 8 macro parameters.
* Prepare MIDI CC output if needed.

6. `PluginEditor`

Responsibilities:

* Own or coordinate `BoardState`, `MappingEngine`, `BoardComponent`, and `OutputRouter`.
* Display the 9x9 board.
* Display 8 macro values.
* Provide Clear and Randomize buttons.
* Provide OSC host/port UI later.
* Recalculate macro values whenever the board changes.

## Hard restrictions

Do not add:

* PyTorch
* ONNX
* TensorFlow
* neural network inference
* FPGA code
* Daisy Seed code
* STM32 code
* sensor hardware code
* complex synthesizer DSP
* RNBO
* WebView
* Vite
* Max/MSP export code

Do not invent JUCE APIs.
If unsure about JUCE API usage, inspect existing JUCE examples or the referenced repositories before writing code.

## First milestone

Only build the minimal controller plugin.

Acceptance criteria:

1. The project configures with CMake.
2. The Standalone target builds.
3. The VST3 target builds.
4. The plugin opens.
5. A 9x9 Go board is visible.
6. Cells can be toggled between Empty, Black, and White.
7. 8 macro values update when the board changes.
8. OSC messages are sent to `127.0.0.1:9000`.
9. MIDI CC output is either implemented simply or clearly stubbed.
10. No synth engine, AI, FPGA, or hardware code is included.
