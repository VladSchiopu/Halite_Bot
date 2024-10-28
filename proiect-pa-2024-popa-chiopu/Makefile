BROWSER?=firefox
BOT_NAME=BotulPopaSchiopu

.PHONY: environment/halite
environment/halite:
	cd environment; make

halite: environment/halite
	cp environment/halite halite


$(BOT_NAME): $(BOT_NAME).cpp


.PHONY: build
build: $(BOT_NAME)

.PHONY: run
run:
	@./$(BOT_NAME)

.PHONY: clean
clean:
	rm $(BOT_NAME)


.PHONY: simulation
simulation: $(BOT_NAME)
	./halite -d "24 28" "./$(BOT_NAME)"  ./bots/DBotv4_linux_x64; 


.PHONY: check1
check1: $(BOT_NAME)
	python3 run.py --cmd ./$(BOT_NAME) --round 1

.PHONY: check2
check2: $(BOT_NAME)
	python3 run.py --cmd ./$(BOT_NAME) --round 2

.PHONY: check3
check3: $(BOT_NAME)
	python3 run.py --cmd ./$(BOT_NAME) --round 3


etapa1.zip: Makefile $(BOT_NAME).cpp networking.hpp hlt.hpp README.md
	zip $@ $^

etapa2.zip: Makefile $(BOT_NAME).cpp networking.hpp hlt.hpp README.md
	zip $@ $^

etapa3.zip: Makefile $(BOT_NAME).cpp networking.hpp hlt.hpp README.md
	zip $@ $^


.PHONY: clean-all
clean-all:
	git clean -dfX
