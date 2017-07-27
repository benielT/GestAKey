#!/bin/bash
kill $(ps aux | grep '[p]ython3 data_read.py' | awk '{print $2}')
