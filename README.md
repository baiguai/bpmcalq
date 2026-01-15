# Time to BPM Calculator

A dark mode GUI application for calculating BPM from time duration, built with C++ and GTKmm.

## Features

- **Dark Mode Interface**: White text on black background for easy viewing
- **Full Keyboard Navigation**: Operate the entire application without a mouse
  - `F1`: Focus Time Signature selector
  - `F2`: Focus Number of Bars selector  
  - `F3`: Focus Time Entry field
  - `F4`: Focus Browse button
  - `F5`: Focus Calculate button
  - `Enter`: Calculate BPM
  - `Escape`: Exit application
- **Time Signature Support**: Common signatures from 2/4 to 12/8
- **Flexible Input**: Enter time in seconds or load audio files
- **Comprehensive Output**: 
  - BPM calculation
  - All note lengths in milliseconds
  - Tempo classification (Largo, Adagio, etc.)
  - Additional timing information

## Requirements

### Ubuntu/Debian
```bash
sudo apt update && sudo apt install -y libgtkmm-3.0-dev libgtk-3-dev pkg-config
```

### Fedora/CentOS/RHEL
```bash
sudo dnf install gtkmm30-devel gtk3-devel pkgconfig
```

### Arch Linux
```bash
sudo pacman -S gtkmm3 gtk3 pkgconf
```

### OpenSUSE
```bash
sudo zypper install gtkmm3-devel gtk3-devel pkg-config
```

## Building and Running

1. Install the required development packages (see above)
2. Build the application:
   ```bash
   ./build.sh
   ```
3. Run the application:
   ```bash
   ./run.sh
   ```

## Usage

1. **Select Time Signature**: Choose your time signature (4/4, 3/4, etc.)
2. **Set Number of Bars**: Enter how many bars your timing covers
3. **Input Time**: 
   - Enter time directly in seconds, or
   - Click "Browse" to select a .wav/.mp3 file
4. **Calculate**: Press Enter or click "Calculate BPM"
5. **View Results**: See BPM and note durations in milliseconds

## Output Information

- **BPM**: Beats per minute calculation
- **Note Lengths**: Duration in milliseconds for:
  - Whole, Half, Quarter, Eighth, Sixteenth notes
  - Thirty-second, Sixty-fourth notes
  - Dotted Quarter, Dotted Eighth notes
  - Triplet Quarter, Triplet Eighth notes
- **Tempo Classification**: Musical tempo term (Largo, Adagio, Andante, etc.)
- **Timing Details**: Total time, bars, beats per bar, total beats

## Technical Details

- **Language**: C++17
- **GUI Framework**: GTKmm 3.0
- **Build System**: CMake
- **Audio Support**: File browsing (duration detection placeholder)