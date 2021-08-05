# The bane of Crash's existance.

debs: web-folders
	bash gen_deb.sh all

web-folders:
	mkdir -p web web/docs web/apt

upload: debs
	cp debs/* web/apt

# Always explicitly specify what you're deleting with rm -rf
clean:
	rm -rf ./web ./debs

.PHONY: clean

default: debs
