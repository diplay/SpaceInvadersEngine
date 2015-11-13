all:
	mkdir distr
	export ADDITIONAL
	+cd src/Engine; make $(MFLAGS)
	+cd src/Invaders; make
	cp src/Invaders/SpaceInvaders distr/SpaceInvaders
	cp res/* distr

clean-all: clean
	cd src/Engine; make clean
	cd src/Invaders; make clean;

clean:
	rm -rf distr
