
private_config_source = service-id.txt \
	service-secret.txt \
	service-url-client.txt \
	service-url-secret.txt \
	service-url-device-user-code-param.txt \
	service-url-lmd-oauth-token-param.txt \
	url-id.txt \
	url-service.txt


if OC_ENABLE_DIST_PRIVATE
EXTRA_DIST = $(private_config_source)
endif

private-config-files: $(private_config_source) 
	@echo $(private_config_source)

.PHONY: private-confi-source

# vi: se ts=4 sw=4 noet:
