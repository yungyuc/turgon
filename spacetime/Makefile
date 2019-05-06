# Build spacetime Python extension:
#   make
# Run Python tests:
#   make pytest
# Run Google tests:
#   make gtest
# Run all tests:
#   make test
# Build verbosely:
#   make VERBOSE=1

USE_CLANG_TIDY ?= OFF
CMAKE_BUILD_TYPE ?= Release
BUILD_PATH ?= build/dev37
SPACETIME_ROOT ?= $(PWD)
VERBOSE ?=

PYTEST ?= $(shell which py.test-3)
ifeq ($(PYTEST),)
	PYTEST := $(shell which pytest)
endif
ifneq ($(VERBOSE),)
	PYTEST_OPTS ?= -v
else
	PYTEST_OPTS ?=
endif

pyextsuffix := $(shell python3-config --extension-suffix)

.PHONY: default
default: $(SPACETIME_ROOT)/libst/_libst$(pyextsuffix)

.PHONY: test
test: gtest pytest

.PHONY: gtest
gtest: $(BUILD_PATH)/gtests/libst_gtests
	$(BUILD_PATH)/gtests/libst_gtests

.PHONY: pytest
pytest: $(SPACETIME_ROOT)/libst/_libst$(pyextsuffix)
	env PYTHONPATH=$(SPACETIME_ROOT) $(PYTEST) $(PYTEST_OPTS) tests/

.PHONY: cmake
cmake: $(BUILD_PATH)/Makefile

$(BUILD_PATH)/gtests/libst_gtests: $(BUILD_PATH)/Makefile
	make -C $(BUILD_PATH) VERBOSE=$(VERBOSE) libst_gtests
	touch $@

$(BUILD_PATH)/_libst$(pyextsuffix): $(BUILD_PATH)/Makefile
	make -C $(BUILD_PATH) VERBOSE=$(VERBOSE) _libst
	touch $@

$(SPACETIME_ROOT)/libst/_libst$(pyextsuffix): $(BUILD_PATH)/_libst$(pyextsuffix)
	make -C $(BUILD_PATH) VERBOSE=$(VERBOSE) install
	touch $@

$(BUILD_PATH)/Makefile: CMakelists.txt Makefile
	cd $(BUILD_PATH) ; \
	cmake $(SPACETIME_ROOT) \
		-DCMAKE_INSTALL_PREFIX=$(SPACETIME_ROOT) \
		-DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) \
		-DUSE_CLANG_TIDY=$(USE_CLANG_TIDY) -DLINT_AS_ERRORS=ON