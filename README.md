# Exercises

For a more detailed description, please visit <https://sconedocs.github.io/print-arg-env/>.

## Determine `sgx device`

The name of the SGX device has changed over time. These days, the SGX driver is part of the Linux kernel. The device is ...

```bash
source determine_sgx_device.sh
```

## Start Cross-Compiler in Docker Container

Run the cross-compiler in a docker container:

```bash
docker run $MOUNT_SGXDEVICE --platform linux/amd64 --network=host -it -v `pwd`:/work registry.scontain.com/sconecuratedimages/crosscompilers bash
```

The next steps are executed inside the container.

## Creating a SCONE CAS policy

We assume that we do not have access to an SGX device and hence, we run in simulation mode.

Sessions can include environment variables like `$SESSION` in the following policy:

```yaml
name: $SESSION
version: 0.3.10

# Disable attestation in case we do not have
# access to SGX or a LAS, see
#    https://sconedocs.github.io/CAS_session_lang_0_3/#disabling-attestation
attestation:
    mode: node

services:
   - name: scone-print-arg-env
     command: ./scone-print-arg-env arg1 arg2 arg3
     environment:
        SCONE_MODE: auto
        env1: running
        env2: in
        env3: enclave
     pwd: /
```

The SCONE CLI can replace these environment variables with values that are passed as arguments
to the SCONE CLI or that are defined in the environment of the SCONE CLI:  

export SESSION=secure-arguments-example-sim-mode-$RANDOM-$RANDOM

## Attesting SCONE CAS

```bash
export SCONE_CAS_ADDR=scone-cas.cf
scone cas attest $SCONE_CAS_ADDR --only_for_testing-debug --only_for_testing-ignore-signer --only_for_testing-trust-any --accept-configuration-needed --accept-sw-hardening-needed --accept-group-out-of-date
```

## Creating a security policy (aka session)

We can create a new policy with the command `scone session create`. The available flags for creating a policy can be printed via flag `--help`:

```bash
scone session create --help
```

We can set the name of the policy in three different ways:

- `--name $POLICY_NAME` overwrites the policy name with the value of the environment variable `POLICY_NAME`
- `-e SESSION=$SESSION_NAME` overwrites the policy name with the value of the environment variable `SESSION_NAME`
- `--use-env` uses the value of the environment variable `SESSION` to set the policy name.

Note that if none of the above options is used, the session creation will fail with the error:

```text
Caused by: Parsing session from session template file 'session.yaml' failed
Caused by: name: Value for variable $SESSION is missing at line 3 column 7
```

This will also be the case if we replace the policy name with flag `--name` since the environment variables are first replaced before the CLI overwrites the policy name with option `--name`.

When defining both `--use-env` and `-e SESSION=$SESSION_NAME`, option `-e` has priority over the environment variables imported via `--use-env`.

Execute:

```bash
scone session create session.yaml
```

### Execute the program

We can now execute the program as follows:

```bash
SCONE_CONFIG_ID=$SESSION/scone-print-arg-env ./scone-print-arg-env
```

## OTP Definitions

### Namespace

We create now a new namespace:

```bash
export SCONE_CAS_ADDR=scone-cas.cf
export NAMESPACE="otp-example-$RANDOM-$RANDOM-$RANDOM-$RANDOM"
export NAMESPACE_PREDECESSOR="~"
export NAMESPACE_PREDECESSOR=$(scone session create --use-env namespace.yaml)
echo NAMESPACE=$NAMESPACE
echo The SESSION HASH of $NAMESPACE  is $NAMESPACE_PREDECESSOR
scone session read $NAMESPACE
```

### OTP Secret

We create now a new OTP secret:

```bash
export OTP_POLICY_PREDECESSOR="~"
# create policy $NAMESPACE/otp_policy
export OTP_POLICY_PREDECESSOR=$(scone session create --use-env otp_policy.yaml)
echo The SESSION HASH of $NAMESPACE/otp_policy is $OTP_POLICY_PREDECESSOR
scone session read $NAMESPACE/otp_policy
unset SCONE_SERVICE_ACCESS_TOKEN
SCONE_CONFIG_ID=$NAMESPACE/otp_policy/print-otp-secret ./scone-print-arg-env
```

### Create Access Token for Namespace


```bash
export ACCESS_TOKEN_POLICY_PREDECESSOR="~"
export ACCESS_TOKEN_POLICY_PREDECESSOR=$(scone session create --use-env access_token_policy.yaml)
echo The SESSION HASH of $NAMESPACE/access_token_policy is $ACCESS_TOKEN_POLICY_PREDECESSOR
scone session read $NAMESPACE/access_token_policy
export OTP=12345
SCONE_CONFIG_ID=$NAMESPACE/access_token_policy/print_access_token@${OTP} ./print-access-token
export OTP= # provide current OTP
export SCONE_SERVICE_ACCESS_TOKEN=$(SCONE_CONFIG_ID=$NAMESPACE/access_token_policy/print_access_token@${OTP} ./print-access-token)
echo $SCONE_SERVICE_ACCESS_TOKEN
```


### Nested Namespace

We create now a new namespace:

```bash
export NESTED_NAMESPACE_PREDECESSOR="~"
export NESTED_NAMESPACE_PREDECESSOR=$(scone session create --use-env nested_namespace.yaml)
echo The SESSION HASH of $NAMESPACE/nested-namespace  is $NESTED_NAMESPACE_PREDECESSOR
scone session read $NAMESPACE/nested-namespace
```

### Import and Use Service Access Token in Nested Policy

```bash
export NESTED_ACCESS_TOKEN_POLICY_PREDECESSOR="~"
export NESTED_ACCESS_TOKEN_POLICY_PREDECESSOR=$(scone session create --use-env nested_access_token_policy.yaml)
echo The SESSION HASH of $NAMESPACE/nested-namespace/access_token_policy is $NESTED_ACCESS_TOKEN_POLICY_PREDECESSOR
scone session read $NAMESPACE/nested-namespace/access_token_policy
export OTP= # provide current OTP
SCONE_CONFIG_ID=$NAMESPACE/nested-namespace/access_token_policy/print_access_token@${OTP} ./print-access-token
```

### Access and Security Policies

```bash
export ACCESS_POLICY_PREDECESSOR="~"
export ACCESS_POLICY_PREDECESSOR=$(scone session create --use-env access_policy.yaml)
echo The SESSION HASH of $NAMESPACE/access_policy is $ACCESS_POLICY_PREDECESSOR
scone session read $NAMESPACE/access_policy
```

### Host Arguments and Variabless

```bash
export HOST_ARGUMENTS_POLICY_PREDECESSOR="~"
export HOST_ARGUMENTS_POLICY_PREDECESSOR=$(scone session create --use-env host_arguments_policy.yaml)
echo The SESSION HASH of $NAMESPACE/host_arguments_policy is $HOST_ARGUMENTS_POLICY_PREDECESSOR
scone session read $NAMESPACE/host_arguments_policy
export OTP=
SCONE_CONFIG_ID=$NAMESPACE/host_arguments_policy/scone-print-arg-env@$OTP ./scone-print-arg-env first last 1234567
```
