#!/bin/sh

node-pre-gyp configure
node-pre-gyp build
node-pre-gyp package
node-pre-gyp-github publish
