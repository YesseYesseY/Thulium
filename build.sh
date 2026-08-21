#!/usr/bin/bash
WINEDEBUG=-all msbuild /nologo "/clp:ErrorsOnly;Summary"
