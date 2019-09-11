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

For developers, `pip` supports an editable install mode that links the source code to the Python `site-packages` directory.
It is strongly recommended to use a `venv` environment or Docker image for development.
To perform a "developer install" using a virtual environment:

```sh
python3 -m venv venv
. venv/bin/activate
pip install -e .
```

This will install both the `vrtgen` Python modules and the `vrtpktgen` executable.

## Testing

The included test suite verifies the YAML parser and Python data model.
To run the test suite, from within the base directory:

```sh
pytest
```

## Linting and exceptions
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


## Anand's notes CentOS 7 local
Testing the runner setup with local gitlab-runner.
Install the gitlab-runner through the system package

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

Now execute the `test` stage defined in the `.gitlab-ci.yaml` file through execution
of `gitlab-runner` using `docker` executor

`gitlab-runner exec docker test`

If you receive error `Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock`, add the `docker` group to your current session. You do not need to log out or reboot!
`newgrp docker`

Run the command again.






