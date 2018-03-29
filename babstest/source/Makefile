
myvar := haha

ifeq ($(myvar),haha)
 $(info myvar is haha)
else
 $(info myvar equals: '$(myvar)')
endif

define MYFUN
  $(info $1 !)
endef

mytgt: HeaderMessage := mytgt BEGIN
mytgt: FooterMessage := mytgt END 
mytgt: header_mytgt yourtgt one two three
	@echo MyTgt
	@echo $(FooterMessage)

yourtgt: HeaderMessage := yourtgt BEGIN
yourtgt: FooterMessage := yourtgt END 
yourtgt: header_yourtgt one two three
	@echo YourTgt
	@echo $(FooterMessage)

one:
	@echo One

two:
	@echo TWo 

three:
	@echo Three


.PHONY: test
test:
	$(call MYFUN,hey)


.PHONY: header_%
header_%:
	@echo $(HeaderMessage)

.PHONE: run
run:
	cd
	@pwd

