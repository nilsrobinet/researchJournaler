# resJourn - Research Journal Application

A Qt6-based research and journal management application for organizing AI interactions, bibliographic references, and general notes.

## Features

### 1. **Gen-AI Tab**
- Track AI prompts and results with multiline text input
- Add notes to document context and insights about AI interactions
- Automatically timestamps all entries

### 2. **Reference Tab (Ref)**
- Manage BibTeX references with support for multiple entry types:
  - **Article** (journal publications)
  - **Book** (books and book chapters)
  - **Inproceedings** (conference papers)
  - **Misc** (miscellaneous sources)
  - **Manual** (user manuals and documentation)
  - **Patent** (patent entries)
- Dynamic field visibility based on selected entry type
- Auto-generated citation keys based on author, title, and year
- Complete BibTeX field support (author, title, journal, pages, DOI, URL, etc.)

### 3. **Notes Tab**
- General-purpose notes entry for project-wide documentation
- Multiline text input with full formatting support

### 4. **Entries Tab**
- View all entries in a sortable table with columns:
  - **Date/Time**: Timestamp of entry creation
  - **Entry**: Preview of entry content with type indicator
  - **Notes**: Additional notes or extracted metadata
- **Edit** entries to modify existing data
- **Remove** entries from the journal
- Color-coded entries by type for visual organization
- Resizable columns for optimal viewing

### 5. **Data Management**
- **Save/Load**: JSON-based persistence for all entries
- **Export BibTeX**: Export reference entries as `.bib` files (compatible with LaTeX/Overleaf)
- **Export Markdown**: Export all entries as formatted Markdown with support for:
  - Structured formatting for different entry types
  - Automatic tab-to-space conversion for code compatibility

### 6. **Configuration**
- Toggle **color-coding** of entries for visual distinction
- Settings persist across sessions using QSettings

## Prerequisites

- Qt6 development libraries
- CMake 3.21 or higher
- C++17 compatible compiler

### Installation on Ubuntu/Debian

```bash
sudo apt-get install qt6-base-dev cmake build-essential
```

### Installation on macOS

```bash
brew install qt cmake
```

### Installation on Windows

Download Qt6 from [qt.io](https://www.qt.io/download) or use vcpkg:
```bash
vcpkg install qt6:x64-windows
```

## Building

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the application
cmake --build .
```

## Running

```bash
./resJourn
```

Or from the build directory:
```bash
cd build
./resJourn
```

## Usage

### Adding a Gen-AI Entry
1. Switch to the **Gen-AI** tab
2. Enter your prompt in the first field
3. Enter the AI result in the second field
4. (Optional) Add notes in the notes field
5. Click **Submit**

### Adding a Reference
1. Switch to the **Ref** tab
2. Select the entry type (Article, Book, etc.) - fields will update automatically
3. Fill in the required fields (Author, Title, etc.)
4. Click **Submit** - citation key will be auto-generated
5. Export to BibTeX when ready

### Adding General Notes
1. Switch to the **Notes** tab
2. Enter your notes
3. Click **Submit**

### Managing Entries
1. Switch to the **Entries** tab
2. View all entries in the table
3. **Edit**: Select an entry and click Edit to modify
4. **Remove**: Select an entry and click Remove to delete
5. **Export**: Choose Markdown or BibTeX export for your entries

## File Format

### Storage (JSON)
Entries are stored in `entries.json` with the following structure:
```json
[
  {
    "type": "Gen-AI",
    "timestamp": "2026-02-28T15:30:00",
    "content": "===PROMPT===\n...\n===RESULT===\n...\n===NOTES===\n..."
  }
]
```

### BibTeX Export
```bibtex
@article{smith2020,
  author = {John Smith},
  title = {Example Article},
  journal = {Example Journal},
  year = {2020}
}
```
