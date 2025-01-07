
private_config_source = github-mail-bind-url.txt \
	service-id.txt \
	service-secret.txt \
	service-url-client-lmd.txt \
	service-url-client-web.txt \
	service-url-device-user-code-param.txt \
	service-url-lmd-oauth-token-param.txt \
	service-url-secret-lmd.txt \
	url-id.txt \
	url-service-lmd.txt \
	url-service-web.txt \
	url-token-query-url.txt 

if OC_ENABLE_DIST_PRIVATE
EXTRA_DIST = $(private_config_source)
endif

private-config-files: $(private_config_source) 
	@echo $(private_config_source)

.PHONY: private-config-files

# vi: se ts=4 sw=4 noet:
