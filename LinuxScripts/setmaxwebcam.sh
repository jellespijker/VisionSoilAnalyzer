#!/bin/bash
v4l2-ctl --device=$1 --set-fmt-video=width=2048,height=1536,pixelformat=MJPG
