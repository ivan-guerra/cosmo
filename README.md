# cosmo

cosmo is a hobby x86 OS. The goal of the project is to implement a very basic
OS capable of running a Userland application from the ground up.

The table below shows what modules have been implemented to date. As the
project progresses, additional modules will be added.

| Module                   | Completed |
| ------------------------ | --------- |
| Framebuffer Driver       | Y         |
| Serial Port Driver       | Y         |
| Logger                   | Y         |
| GDT                      | Y         |
| IDT                      | Y         |
| PIC Driver               | Y         |
| Physical Frame Allocator | Y         |
| Virtual Memory Manager   | N         |
| User Mode Process        | N         |

Development has leaned heavily on the ["The litte book about os
development"](https://littleosbook.github.io/) as well as the the
[OSDev](https://wiki.osdev.org/Main_Page) forums as references. Plenty of the
source files and commit messages link back to reference material.

### Running cosmo

cosmo was developed on a Linux PC using [Bochs](https://bochs.sourceforge.io/)
for x86 emulation. The cross compiler, emulator, and additional packages needed
to build and run cosmo are all provided within a docker container. To run
cosmo in a container you will need:

* [Docker](https://docs.docker.com/engine/install/)
* A Linux Host with an X11 Window System installed (Xming on Windows is also
an option though it has not been tested)

To launch the container:
```
scripts/create_container.sh
```

Once the container is created, you will be dropped into a terminal that
contains the project files as hosted on the host PC. From here, you can build
and run the OS in the Bochs emulator:

To build cosmo:
```
scripts/build_cosmo.sh
```

To run the emulator:
```
scripts/run_cosmo.sh
```

If the above two commands succeed, you should see a Bochs window like the one
shown below.

![cosmo](res/cosmo.png)

### Project Documentation

Project docs can be viewed in HTML. To build the project documentation,
pass the `-d` flag to the [`scripts/build_cosmo.sh`](scripts/build_cosmo.sh)
script:

```
scripts/build_cosmo.sh -d
```

After running the above command, cosmo docs will be installed to the project
root directory under `docs/cosmo_docs`.
