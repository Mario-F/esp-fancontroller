# Fan Controller

## Idea and initial sources

Fancontrol: https://www.youtube.com/watch?v=UJK2JF8wOu8

## Quickstart

Create a `src/Crendials.h` with your wifi credentials, use `src/Crendials.h.tpl`

On remote Host:

```shell
pio remote agent start
```

Connect Monitor:

```shell
pio remote device monitor -b 115200
```

Upload and Flash remote:

```shell
pio remote run -t upload --force-remote
```
