# Valvers Website

The mkdocs source code for the valvers.com website.

## License

Everything here is licensed under the [MIT](https://opensource.org/licenses/MIT) license.

I was going to go for the CC0 license - but it's too much of a faf and the MIT license
was as close as I could get to public domain.

## Building

Get the build dependencies and then run `mkdocs build OUTPUT_DIR [API_TOKEN]`

where the `OUTPUT_DIR` is the destination directory for the website and `API_TOKEN`
is an optional personal api access token for the git committers plugin.

### Fedora

```
python -m pip install mkdocs mkdocs-material mkdocs-git-committers-plugin --user
```

This gets you the mkdocs binary installed locally for your user:

```
$ which mkdocs
~/.local/bin/mkdocs
```

## Developing

Local development is done by adding/modifying the contents of the `docs` directory.

The site can be built with real-time change monitoring for developing changes by using

```
$ mkdocs serve
INFO    -  Building documentation... 
INFO    -  Cleaning site directory 
[I 200218 10:40:10 server:296] Serving on http://127.0.0.1:8000
[I 200218 10:40:10 handlers:62] Start watching changes
```
