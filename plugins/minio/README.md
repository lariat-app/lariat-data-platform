# MinIO Blob Storage

MinIO plugin for Lariat Data Platform

## Usage

An `LdpBlobStorage` resource can be defined with the `minio` provider 
and `properties`:

```json
{
    "provider" : "minio",
    "properties" : {
        "base_url" : "play.min.io",
        "credentials" : {
            // see description under Credentials section
        }
    }
}
```

The following properties are supported:

+ `base_url` - Base URL for all S3 endpoints. Can be prefixed by 
`http://` or `https://`, but will use the latter as a default.
+ `credentials` - Defines one of the credential types used to 
authenticate the client.

### Credentials

The MinIO client is intended to be flexible with its credentials, 
so the configuration has been mapped accordingly:

+ `static` - hard coded access and secret keys, with optional session 
token:

```json
{
    "credentials" : {
        "static" : {
            "access_key" : "Q3AM3UQ867SPQQA43P2F",
            "secret_key" : "zuf+tfteSlswRu7BJ86wekitnifILbZam1KYY3TG"
        }
    }
}
```