global readTimeStampCounter

    readTimeStampCounter:        ;Reads the CPU time stamp counter
        rdtsc                    ;Reads the time stamp counter into edx:eax
        ret                      ;Returns to the calling function