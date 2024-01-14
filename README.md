# Task 1

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

We assume that we do not have access to an SGX device and hence, we run in simulation mode

```bash
export SESSION=secure-arguments-example-sim-mode-$RANDOM-$RANDOM
cat > session.yaml <<EOF
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
EOF
```

## Attesting SCONE CAS

```bash
export SCONE_CAS_ADDR=scone-cas.cf
scone cas attest $SCONE_CAS_ADDR --only_for_testing-debug --only_for_testing-ignore-signer --only_for_testing-trust-any --accept-configuration-needed --accept-sw-hardening-needed --accept-group-out-of-date
```

## Creating the policy

Execute:

```bash
scone session create session.yaml
```

### Execute the program

We can now execute the program as follows:

```bash
SCONE_CONFIG_ID=$SESSION/scone-print-arg-env /work/scone-print-arg-env
```
