.SUFFIXES:

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


.PHONY: header_mtgt header_yourtgt
header_%:
	@echo $(HeaderMessage)

.PHONY: myphony
myphony:
	@echo myphony

mytesttgt: myphony
	touch mytesttgt

