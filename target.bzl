load(":video_modules.bzl", "video_driver_modules")
load(":video_driver_build.bzl", "define_lunch_target_variant_modules")
load("//soc-repo:target_variants.bzl", "la_target_variants")

def define_target_modules():
    for (target, variant) in la_target_variants():
        define_lunch_target_variant_modules(
            target = target,
            variant = variant,
            registry = video_driver_modules,
            modules = [
                "msm_video",
                "video",
            ],
        )
