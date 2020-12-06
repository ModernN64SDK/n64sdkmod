# The bane of Crash's existance.

SSH-SERVER ?= coneyislanddiscopalace.xyz

debs:
	bash gen_deb.sh all

web-folders:
	mkdir -p web web/docs web/apt

docs: web-folders
	markdown INSTALL.md > web/docs/index.html
	markdown DOCKER.md > web/docs/docker.html

upload: docs debs
	cp debs/* web/apt

# Always explicitly specify what you're deleting with rm -rf
clean:
	rm -rf ./web ./debs

.PHONY: clean

default: debs
