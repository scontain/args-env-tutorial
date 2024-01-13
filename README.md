# Task 1

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

In the container exec

## Creating a session

We assume that we do no have access to a SGX device and run in simulation mode

```bash
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
scone cas attest $SCONE_CAS_ADDR
```

