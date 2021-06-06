# Fan Controller

## Idea and initial sources

Fancontrol: https://www.youtube.com/watch?v=UJK2JF8wOu8

## Quickstart

Create a `src/Credentials.h` with your wifi credentials, use `src/Credentials.h.tpl`

On remote Host:

```shell
pio remote agent start
```

Connect Monitor:

```shell
pio remote device monitor -b 115200
```

Upload Filesystem:

```shell
pio remote run -t uploadfs --force-remote
```

Upload and Flash remote:

```shell
pio remote run -t upload --force-remote
```

## Test

To use the http calls provided by the `test/` folder you can use vscode extension `REST Client` and this config under `.vscode/settings.json`:

```json
{
  "rest-client.environmentVariables": {
    "$shared": {
        "fanControllerHost": "1.2.3.4"
    }
  }
}
```
