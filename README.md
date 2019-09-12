# VITA 49.2 Packet Generator

Generates native-language classes/structs for V49.2 packet definitons in a YAML configuration file.

This is a work-in-progress and almost assuredly will break repeatedly.

## Dependencies

`vrtgen` requires Python 3.6 or newer with the following packages:

* PyYAML (3.11 or newer)
* Jinja2 (2.8 or newer)

For developers, the following packages are recommended:

* pytest
* pylint

## Installation

Installation via `pip` is recommended, as it will automatically install the required dependencies.
To install via `pip`:

```sh
pip3 install .
```

_Note_: Installing to the default system location may require superuser privileges.

## Usage

The package installs a `vrtpktgen` executable in your path.
By default, `vrtpktgen` only validates a YAML packet definition file.

```sh
vrtpktgen <path-to-file.yml>
```

The `vrtpktgen` program supports backend plug-ins to target different output types.
Select an available backend with the `-b <backend>` flag.
The `bindump` backend is included with the default install.
For example:

```sh
vrtpktgen -b bindump <path-to-file.yml>
```

will give a hexadecimal dump of the default configuration for each packet defined in the YAML file.

A C++ backend is available in the `cpp` subdirectory of the source.
For more information, see [cpp/README.md](./cpp/README.md).

## Developer Setup
Ideally, developers make use of the same build environment as defined in the [.gitlab-ci.yml](./gitlab-ci.yml) file.

### CentOS 7
#### Docker instllation and configuraion
The following installs docker and adds the user to the `docker` group.
```bash
yum install yum-utils device-mapper-persistent-data lvm2
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
yum install docker-ce docker-ce-cli containerd.io

# The docker package runs docker service under docker user. In order to
# make use of the docker execution without sudo ensure the user is appended
# to the docker group, must start new shell/session, see below
usermod -aG docker <your username>

# verify the above command worked
cat /etc/group | grep docker   # the output should contain <your username>

# ensure service starts automatically
systemctl enable docker
systemctl start docker
```
Docker is installed and ready to use, happy developing!

#### gitlab-runner installation
The GitLab CI/CD uses `gitlab-runner` tool to execute the stages defined in the
[.gitlab-ci.yml](./.gitlab-ci.yml) file. First you need to install the tool on
your host. Once installed, all the stages can be executed using the tool.

You need to install the `gitlab-runner` tool first. The tool is provided by the
upstream GitLab repository and you are encouraged to consult the current documentation.
Below is an example for the GitLab version 12.2.

```bash
curl -L https://packages.gitlab.com/install/repositories/runner/gitlab-runner/script.rpm.sh > /tmp/script.rpm.sh
chmod +x /tmp/script.rpm.sh
sudo /tmp/script.rpm.sh
sudo yum install gitlab-runner.x86_64
```

[See below](#execute-stages)

##### Advance setup, does not require Docker

For developers, `pip` supports an editable install mode that links the source code to the Python `site-packages` directory.
It is strongly recommended to use a `venv` environment or Docker image for development.
To perform a "developer install" using a virtual environment:

```sh
python3 -m venv venv
. venv/bin/activate
pip install -e .
```

This will install both the `vrtgen` Python modules and the `vrtpktgen` executable.

## Testing and Building

### Overview
The project uses docker to test and build. The project makes use of GitLab CI/CD
to run those stages upon every push. However, contributors are encouraged to
perform those steps locally prior to pushing. The build stages are defined in the
[.gitlab-ci.yml](./gitlab-ci.yml) that the CI executes on a push event.

#### Execute test locally
The local setup is same as the CI setup defined in the .gitlab-ci.yaml. Start a
docker container, mount the current dir path returned by `pwd` as `/vrtgen` into
the container.
`docker run -it --rm -v `pwd`:/vrtgen python:3.6-alpine sh`

You should have a shell prompt inside docker. Change the directoryi `cd /vrtgen`
and continue development. Files modifications are instantly visible on both the
container and the host.

#### Execute stages
If you prefer to execute all the stages defined in the [.gitlab-ci.yml](./gitlab-ci.yml)
file, use of `gitlab-runner` tool is recommended.
Once the `gitlab-runner` tool is installed, execute the following to run all the stages.
```bash
gitlab-runner exec docker
```

Individual stages can be executed for the more focused and shorter execution time. The
following command only executes the `lint` stage.
```bash
gitlab-runner exec docker lint
```


### Linting and exceptions
The `pylint` can get in a way in some cases. Rather then disabling lint warning globally,
an exception can be added inline at the site of warning.
For e.g., the code below issues warning `C0301: Line too long (101/100) (line-too-long)`

```python
self._action = self._add_field(ControlAcknowledgeMode.action, Scope.PROLOGUE, Mode.MANDATORY)
```

The warning can be suppressed on a case-by-case basis via a `comment header` as the following:

```python
# pylint: disable=C0301
self._action = self._add_field(ControlAcknowledgeMode.action, Scope.PROLOGUE, Mode.MANDATORY)
```


## Troubleshooting
 * If you receive the error `Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock`, execute `newgrp docker` to add the `docker` group to your current session/shell . You do not need to log out or reboot!




