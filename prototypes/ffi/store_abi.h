// Experimental C ABI for comparing storage-prototype interoperability.
// Not a stable engine interface or distribution contract.
#ifndef CAMPISTORIA_PROTOTYPE_STORE_ABI_H
#define CAMPISTORIA_PROTOTYPE_STORE_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

void* campistoria_store_new(void);
void campistoria_store_free(void* handle);
int campistoria_store_append(void* handle, int stream, const char* event_json);
int campistoria_store_count(void* handle, int stream);

#ifdef __cplusplus
}
#endif

#endif
