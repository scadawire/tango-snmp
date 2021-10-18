#
# CAR_OCI_REGISTRY_HOST and PROJECT are combined to define
# the Docker tag for this project. The definition below inherits the standard
# value for CAR_OCI_REGISTRY_HOST = artefact.skao.int and overwrites
# PROJECT to give a final Docker tag of
# artefact.skao.int/ska-low-cbf-fpga/powersupply
#
PROJECT = ska-low-cbf-snmp

# KUBE_NAMESPACE defines the Kubernetes Namespace that will be deployed to
# using Helm.  If this does not already exist it will be created
KUBE_NAMESPACE ?= ska-low-cbf-snmp

# RELEASE_NAME is the release that all Kubernetes resources will be labelled
# with
RELEASE_NAME ?= test

# UMBRELLA_CHART_PATH Path of the umbrella chart to work with
HELM_CHART ?= test-parent
UMBRELLA_CHART_PATH ?= charts/$(HELM_CHART)/

# Fixed variables
# Timeout for gitlab-runner when run locally
TIMEOUT = 86400
# Helm version
HELM_VERSION = v3.3.1
# kubectl version
KUBERNETES_VERSION = v1.19.2

CI_PROJECT_DIR ?= .

KUBE_CONFIG_BASE64 ?=  ## base64 encoded kubectl credentials for KUBECONFIG
KUBECONFIG ?= /etc/deploy/config ## KUBECONFIG location

XAUTHORITY ?= $(HOME)/.Xauthority
THIS_HOST := $(shell ip a 2> /dev/null | sed -En 's/127.0.0.1//;s/.*inet (addr:)?(([0-9]*\.){3}[0-9]*).*/\2/p' | head -n1)
DISPLAY ?= $(THIS_HOST):0
JIVE ?= false# Enable jive
WEBJIVE ?= false# Enable Webjive

CI_PROJECT_PATH_SLUG ?= ska-low-cbf-snmp
CI_ENVIRONMENT_SLUG ?= ska-low-cbf-snmp
$(shell echo 'global:\n  annotations:\n    app.gitlab.com/app: $(CI_PROJECT_PATH_SLUG)\n    app.gitlab.com/env: $(CI_ENVIRONMENT_SLUG)' > gilab_values.yaml)

# define private overrides for above variables in here
-include PrivateRules.mak

#
# include makefile to pick up the standard Make targets, e.g., 'make build'
# build, 'make push' docker push procedure, etc. The other Make targets
# ('make interactive', 'make test', etc.) are defined in this file.
#
include .make/release.mk
include .make/k8s.mk
include .make/make.mk
#include .make/python.mk
include .make/helm.mk
include .make/oci.mk
include .make/help.mk
#include .make/docs.mk

# Chart for testing
K8S_CHARTS = test-parent

# define private overrides for above variables in here
#-include PrivateRules.mak

# Single image in root of project
OCI_IMAGES = ska-low-cbf-snmp

# Test runner - run to completion job in K8s
# name of the pod running the k8s_tests
TEST_RUNNER = test-runner-$(CI_JOB_ID)-$(RELEASE_NAME)

ITANGO_DOCKER_IMAGE = $(CAR_OCI_REGISTRY_HOST)/ska-tango-images-tango-itango:9.3.5

#PYTHON_VARS_BEFORE_PYTEST = PYTHONPATH=src:src/ska_low_cbf_fpga
#PYTHON_VARS_AFTER_PYTEST = -m "not post_deployment"

HELM_CHARTS_TO_PUBLISH ?= ska-low-cbf-snmp

#PYTHON_BUILD_TYPE = non_tag_setup

ifneq ($(CI_JOB_ID),)
K8S_TEST_TANGO_IMAGE = --set ska-low-cbf-snmp.snmp.image.tag=$(VERSION)-dev.$(CI_COMMIT_SHORT_SHA) \
	--set ska-low-cbf-snmp.snmp.image.registry=$(CI_REGISTRY)/ska-telescope/tangosnmp
IMAGE_TO_TEST=$(CI_REGISTRY)/ska-telescope/tangosnmp/ska-low-cbf-snmp:$(VERSION)-dev.$(CI_COMMIT_SHORT_SHA)
else
K8S_TEST_TANGO_IMAGE = --set ska-low-cbf-snmp.snmp.image.tag=$(VERSION)
endif

K8S_CHART_PARAMS = --set global.minikube=$(MINIKUBE) \
	--set global.tango_host=$(TANGO_HOST) \
	--set ska-tango-base.display=$(DISPLAY) \
	--set ska-tango-base.xauthority=$(XAUTHORITY) \
	--set ska-tango-base.jive.enabled=$(JIVE) \
	--set webjive.enabled=$(WEBJIVE) \
	${K8S_TEST_TANGO_IMAGE} \
	--values gilab_values.yaml

#requirements: ## Install Dependencies
#	python3 -m pip install -r requirements-dev.txt


#python-pre-lint: requirements## Overriding python.mk
#
#python-pre-test: requirements## Overriding python.mk
#	@mkdir -p build;
#
#pipeline_unit_test: ## Run simulation mode unit tests in a docker container as in the gitlab pipeline
#	@docker run --volume="$$(pwd):/home/tango/ska-low-cbf-fpga" \
#		--env PYTHONPATH=src:src/ska_low_cbf_fpga --env FILE=$(FILE) -it $(ITANGO_DOCKER_IMAGE) \
#		sh -c "cd /home/tango/ska-low-cbf-fpga && make requirements && make python-test"



#.PHONY: all test help k8s lint logs describe namespace delete_namespace kubeconfig kubectl_dependencies k8s_test install-chart uninstall-chart reinstall-chart upgrade-chart interactive
