NUM ?= 3 # For LaTeX to correctly cross-reference.  To save time, use 1.
VERBOSE ?= 0
DOEPS ?= 1
ifeq ($(VERBOSE),0)
	CMDLOG_DIR_REDIRECT := > /dev/null
	PSTAKE := ./pstake.py -q
else
	CMDLOG_DIR_REDIRECT := >&1
	PSTAKE := ./pstake.py
endif

# tool binaries
LATEX := latex
BIBTEX := bibtex
DVIPS := dvips
PS2PDF := ps2pdf
ifeq ($(DOEPS),0)
	DO_PSTAKE = mkdir -p $(EPS_DIR) ; touch $@
else
	DO_PSTAKE = mkdir -p $(EPS_DIR) ; $(PSTAKE) $< $@
endif

# tool binaries

# directories
LOG_DIR := log
EPS_DIR := turgon_eps
SCHEMATIC_DIR := schematic

HANDOVER_DIR := handover
NOWDATE := $(shell date +"%Y%m%d")
NOWID := $(shell git rev-parse --short HEAD 2> /dev/null || echo "nogit")
HANDOVER_FN = $(HANDOVER_DIR)/$(basename $<).$(NOWID).$(NOWDATE).pdf

TRUNK := turgon

MAKEFILES := Makefile

ALL_TEX := $(wildcard $(SCHEMATIC_DIR)/*.tex)
ALL_EPS := $(patsubst $(SCHEMATIC_DIR)/%.tex,$(EPS_DIR)/%.eps,$(ALL_TEX))

.PHONY: default
default: note mesh

.PHONY: ho
ho: note_ho mesh_ho

$(EPS_DIR)/%.eps: $(SCHEMATIC_DIR)/%.tex $(MAKEFILES) pstake.py
	$(DO_PSTAKE)

.PHONY: note
note: $(TRUNK)_note.pdf

.PHONY: note_ho
note_ho: $(TRUNK)_note.pdf
	@echo "Generating today's PDF: $(HANDOVER_FN)"
	mkdir -p $(HANDOVER_DIR)
	cp -f $< $(HANDOVER_FN)

$(TRUNK)_note.dvi: $(TRUNK)_note.tex $(TRUNK)_main.bib $(ALL_EPS) $(MAKEFILES)
	mkdir -p $(LOG_DIR)
	@echo "Having EPS files: $(ALL_EPS)"
	num=1 ; while [ $$num -le $(NUM) ] ; do \
		$(LATEX) $< 2>&1 | tee $(LOG_DIR)/$@.latex.$$num.log $(CMDLOG_DIR_REDIRECT) ; \
		$(BIBTEX) $(basename $<) 2>&1 | tee $(LOG_DIR)/$@.bibtex.$$num.log $(CMDLOG_DIR_REDIRECT) ; \
	(( num = num + 1 )) ; done

$(TRUNK)_note.ps: $(TRUNK)_note.dvi $(MAKEFILES)
	mkdir -p $(LOG_DIR)
	$(DVIPS) $< 2>&1 | tee $(LOG_DIR)/$@.log $(CMDLOG_DIR_REDIRECT)

$(TRUNK)_note.pdf: $(TRUNK)_note.ps $(MAKEFILES)
	mkdir -p $(LOG_DIR)
	$(PS2PDF) $< 2>&1 | tee $(LOG_DIR)/$@.log $(CMDLOG_DIR_REDIRECT)

.PHONY: mesh
mesh: $(TRUNK)_mesh.pdf

.PHONY: mesh_ho
mesh_ho: $(TRUNK)_mesh.pdf
	@echo "Generating today's PDF: $(HANDOVER_FN)"
	mkdir -p $(HANDOVER_DIR)
	cp -f $< $(HANDOVER_FN)

$(TRUNK)_mesh.dvi: $(TRUNK)_mesh.tex $(TRUNK)_main.bib $(ALL_EPS) $(MAKEFILES)
	mkdir -p $(LOG_DIR)
	@echo "Having EPS files: $(ALL_EPS)"
	num=1 ; while [ $$num -le $(NUM) ] ; do \
		$(LATEX) $< 2>&1 | tee $(LOG_DIR)/$@.latex.$$num.log $(CMDLOG_DIR_REDIRECT) ; \
		$(BIBTEX) $(basename $<) 2>&1 | tee $(LOG_DIR)/$@.bibtex.$$num.log $(CMDLOG_DIR_REDIRECT) ; \
	(( num = num + 1 )) ; done

$(TRUNK)_mesh.ps: $(TRUNK)_mesh.dvi $(MAKEFILES)
	mkdir -p $(LOG_DIR)
	$(DVIPS) $< 2>&1 | tee $(LOG_DIR)/$@.log $(CMDLOG_DIR_REDIRECT)

$(TRUNK)_mesh.pdf: $(TRUNK)_mesh.ps $(MAKEFILES)
	mkdir -p $(LOG_DIR)
	$(PS2PDF) $< 2>&1 | tee $(LOG_DIR)/$@.log $(CMDLOG_DIR_REDIRECT)

.PHONY: clean_tex
clean_tex:
	rm -f \
		$(TRUNK)_*.aux \
		$(TRUNK)_*.bbl \
		$(TRUNK)_*.blg \
		$(TRUNK)_*.dvi \
		$(TRUNK)_*.log \
		$(TRUNK)_*.out \
		$(TRUNK)_*.pdf \
		$(TRUNK)_*.ps

.PHONY: clean_eps
clean_eps:
	rm -rf $(LOG_DIR) $(EPS_DIR)

.PHONY: clean
clean: clean_tex clean_eps
