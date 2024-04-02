# README

### Brief
This is a repo containing a minimal GTKmm 2.4 demo application. However, I may add more features since this is a working project.

### Notes
 - Built on MacOS Sonoma.

### Steps
 1. Ensure you have Homebrew, CMake 3.27+, and Git.
 2. Run `brew install gtkmm`.
    - To check if the install succeeded, run `brew info gtkmm`. All dependencies should be there.
 3. Clone this repository.
    - Create `bin` and `build` folders under the project root.
 4. Run `cmake .`
 5. Run the build command for your native build system `make`, `ninja`, etc.
 6. Run the program at `./bin/demoapp`.

### WIP Features
 1. Allows browsing, adding, and deleting "pages" of plain notes
 2. Allows viewing, adding, and deleting notebooks
 3. Supports saving notes in a custom format.
 4. Can save drafts of pages?
