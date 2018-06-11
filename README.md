<p align="center">
  <img src="/.static/logo.png" />
</p>

<p align="center">
  <a href="https://travis-ci.com/bobheadxi/posit.io">
    <img src="https://travis-ci.com/bobheadxi/posit.io.svg?token=yjpN8EDzhK3qYaBpZzZc&branch=master" alt="Travis Builds" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/bobheadxi/posit.io/milestones"><strong>Milestones</strong></a>
  ·
  <a href="#hammer-building"><strong>Building</strong></a>
</p>

<br/>

## :hammer: Building

```bash
bash scripts/setup.sh  # install dependencies
cmake .                # generate Makefiles
```

To try the current proof-of-concept demonstration:

```bash
make demo
./bin/demo
```

To execute tests:

```bash
make tests
./bin/tests
```
