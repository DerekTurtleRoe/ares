Compiling from source on Linux
------------------------------

You will need a copy of the ares source-code.
If you download an official release from the ares homepage,
you will need [7-zip](http://www.7-zip.org)
or a compatible tool to extract it.
Alternatively,
you may obtain ares source code from
[the unofficial git repo](https://gitlab.com/ares/ares/)
using the Git source-code management tool,
or by clicking the download button on the right-hand side of the web-page
and choosing an archive format.

You will also need GCC 7 or higher,
including the C and C++ compiler,
GNU Make,
and development files
(headers, etc.)
for the following libraries:

  - GTK 2.x
  - PulseAudio
  - Mesa
  - gtksourceview 2.x
  - Cairo
  - SDL 2.0
  - libXv
  - libAO
  - OpenAL
  - udev

On a Debian-derived Linux distribution
(including Ubuntu and Mint),
you can install everything you need with a command like:

    sudo apt-get install build-essential libgtk2.0-dev libpulse-dev \
        mesa-common-dev libgtksourceview2.0-dev libcairo2-dev libsdl2-dev \
        libxv-dev libao-dev libopenal-dev libudev-dev

Once you have all the dependencies installed,
you may build and install ares.

Note: Run these commands as yourself,
**do not run them as root**
(no `sudo`, no `su`, etc.),
because ares does not support
being installed system-wide.

 1. Put the ares source code in some convenient location,
    like `~/ares-src`
 2. Open a terminal window
 3. Type `cd ~/ares-src`
    (or wherever you put the ares source)
    and press Enter
 4. Type `make -C ares target=ares` and press Enter
    to build the main ares executable
 5. Type `make -C icarus` and press Enter
    to build the icarus import tool

Installing a compiled build on Linux
------------------------------------

Assuming you have successfully compiled ares
as described in the previous section:

 1. Open a terminal window
 2. Type `cd ~/ares-src`
    (or wherever you put the ares source)
    and press Enter
 3. Type `make -C ares target=ares install` and press Enter
    to install ares and its supporting files
 4. Type `make -C icarus install` and press Enter
    to install icarus and its game database
 5. If the ares source includes a `shaders` subdirectory,
    type `make -C shaders install`
    to install the example
    [video shaders](../guides/shaders.md).

This installs ares and its associated data files
into the `~/.local` directory hierarchy.

To confirm ares is installed correctly,
type `ares` in a terminal and press Enter.
If the ares window appears,
everything is working.
On the other hand,
if you get an error message like "command not found",
you should double-check that the directory `~/.local/bin`
is included in your `$PATH` environment variable
by running the following command in a terminal:

    echo "$PATH" | tr ':' '\n' | grep ~/.local/bin

If the above command prints the full path of `~/.local/bin`
(for example: `/home/yourname/.local/bin`)
then you should be good.
If it prints nothing,
you need to add the following line to `~/.profile`:

    export PATH=~/.local/bin:$PATH

(this line must be in `~/.profile` because
most GUIs do not read any other files at login)

If you also have a `~/.bash_profile`,
make sure it reads the contents of `~/.profile`
with a line like this:

    source ~/.profile

You will need to log out and log back in
for changes to `~/.profile` or `~/.bash_profile`
to take effect.

Uninstalling a compiled build on Linux
--------------------------------------

To uninstall ares,
as installed by the above instructions:

 1. Open a terminal window
 2. Type `cd ~/ares-src`
    (or wherever you put the ares source)
    and press Enter
 3. Type `make -C ares target=ares uninstall` and press Enter
 4. Type `make -C icarus uninstall` and press Enter

To remove ares's configuration,
you should also delete the following directories
if they exist:

  - `~/.config/ares/`
  - `~/.config/hiro/`
  - `~/.local/share/ares/`
  - `~/.local/share/hiro/`

To remove the games imported into ares's library
(including in-game saves and save-states),
delete the directory `~/Emulation`.

You may also wish to delete the ares source directory.
