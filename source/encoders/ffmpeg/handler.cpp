#include "handler.hpp"
#include "../encoder-ffmpeg.hpp"

streamfx::encoder::ffmpeg::handler::handler_map_t& streamfx::encoder::ffmpeg::handler::handlers()
{
	static handler_map_t handlers;
	return handlers;
}

streamfx::encoder::ffmpeg::handler::handler(std::string codec)
{
	handlers().emplace(codec, this);
}

bool streamfx::encoder::ffmpeg::handler::has_keyframes(ffmpeg_factory* factory)
{
#if LIBAVCODEC_VERSION_MAJOR > 58
	if (auto* desc = avcodec_descriptor_get(factory->get_avcodec()->id); desc) {
		return (desc->props & AV_CODEC_PROP_INTRA_ONLY) == 0;
	} else {
		return false;
	}
#else
	return (factory->get_avcodec()->capabilities & AV_CODEC_CAP_INTRA_ONLY) == 0;
#endif
}

bool streamfx::encoder::ffmpeg::handler::has_threading(ffmpeg_factory* factory)
{
	return (factory->get_avcodec()->capabilities & (AV_CODEC_CAP_FRAME_THREADS | AV_CODEC_CAP_SLICE_THREADS | AV_CODEC_CAP_OTHER_THREADS));
}

bool streamfx::encoder::ffmpeg::handler::is_hardware(ffmpeg_factory* factory)
{
	if (factory->get_avcodec()->capabilities & AV_CODEC_CAP_HARDWARE) {
		return true;
	}
	return false;
}

bool streamfx::encoder::ffmpeg::handler::is_reconfigurable(ffmpeg_factory* factory, bool& threads, bool& gpu, bool& keyframes)
{
	if (factory->get_avcodec()->capabilities & AV_CODEC_CAP_PARAM_CHANGE) {
		return true;
	}
	return false;
}

void streamfx::encoder::ffmpeg::handler::adjust_info(ffmpeg_factory* factory, std::string& id, std::string& name, std::string& codec) {}

std::string streamfx::encoder::ffmpeg::handler::help(ffmpeg_factory* factory) {
	return "about:blank";
}

void streamfx::encoder::ffmpeg::handler::defaults(ffmpeg_factory* factory, obs_data_t* settings) {}

void streamfx::encoder::ffmpeg::handler::properties(ffmpeg_factory* factory, ffmpeg_instance* instance, obs_properties_t* props) {}

void streamfx::encoder::ffmpeg::handler::migrate(ffmpeg_factory* factory, ffmpeg_instance* instance, obs_data_t* settings, uint64_t version) {}

void streamfx::encoder::ffmpeg::handler::update(ffmpeg_factory* factory, ffmpeg_instance* instance, obs_data_t* settings) {}

void streamfx::encoder::ffmpeg::handler::override_update(ffmpeg_factory* factory, ffmpeg_instance* instance, obs_data_t* settings) {}

void streamfx::encoder::ffmpeg::handler::log(ffmpeg_factory* factory, ffmpeg_instance* instance, obs_data_t* settings) {}

void streamfx::encoder::ffmpeg::handler::override_colorformat(ffmpeg_factory* factory, ffmpeg_instance* instance, obs_data_t* settings, AVPixelFormat& target_format) {}
