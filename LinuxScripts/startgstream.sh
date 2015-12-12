#!/bin/bash
# script to start gstreamer string

gst-launch-0.10 v4l2src ! video/x-raw-gray ! tisvideobufferfilter ! tis_auto_exposure ! tiscolorize ! queue ! tiswhitebalance ! queue ! bayer2rgb ! queue ! ffmpegcolorspace ! autovideosink
