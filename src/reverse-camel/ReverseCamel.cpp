/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2015 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPlugin.hpp"

#define DISTTUBE_COEFFS_LENGTH 101

const float disttube_coeffs[][4] = {
    { 1, -0.000207935, 0.999713, -9.81884e-05 },
    { 1.048, 1.993, 4.17732, 1.18892 },
    { 1.08, 1.73605, 3.74991, 1.02497 },
    { 1.12095, 1.47375, 3.30412, 0.851922 },
    { 1.16548, 1.25079, 2.91563, 0.699096 },
    { 1.20649, 1.08731, 2.62376, 0.58282 },
    { 1.24634, 0.957433, 2.38663, 0.487302 },
    { 1.28149, 0.861304, 2.20747, 0.414417 },
    { 1.32251, 0.766302, 2.02684, 0.340243 },
    { 1.36353, 0.686164, 1.87118, 0.275696 },
    { 1.4104, 0.60907, 1.71805, 0.211566 },
    { 1.4479, 0.556382, 1.61117, 0.166404 },
    { 1.48306, 0.512929, 1.52144, 0.12821 },
    { 1.52877, 0.463719, 1.41783, 0.0837588 },
    { 1.56509, 0.429445, 1.34426, 0.051955 },
    { 1.60845, 0.39334, 1.26532, 0.017589 },
    { 1.64829, 0.363999, 1.19994, -0.0110755 },
    { 1.68814, 0.337819, 1.14058, -0.0372625 },
    { 1.7233, 0.316995, 1.09257, -0.0585751 },
    { 1.77252, 0.290956, 1.03144, -0.0858797 },
    { 1.8147, 0.271086, 0.983896, -0.107253 },
    { 1.84635, 0.257505, 0.950883, -0.122171 },
    { 1.89556, 0.238309, 0.903466, -0.14371 },
    { 1.93658, 0.223909, 0.867239, -0.160264 },
    { 1.97056, 0.212935, 0.839216, -0.173129 },
    { 2.00338, 0.203107, 0.813791, -0.184843 },
    { 2.05025, 0.19019, 0.779865, -0.200549 },
    { 2.09362, 0.179302, 0.750776, -0.214083 },
    { 2.12759, 0.171417, 0.729403, -0.224069 },
    { 2.1733, 0.161596, 0.702399, -0.236737 },
    { 2.20963, 0.154387, 0.682277, -0.246212 },
    { 2.24478, 0.147858, 0.66382, -0.254937 },
    { 2.28931, 0.140162, 0.641757, -0.265404 },
    { 2.32446, 0.134513, 0.625333, -0.273225 },
    { 2.35845, 0.129363, 0.610185, -0.28046 },
    { 2.41001, 0.122094, 0.588495, -0.29086 },
    { 2.44048, 0.118085, 0.576366, -0.296694 },
    { 2.49204, 0.11174, 0.556914, -0.306078 },
    { 2.53186, 0.107177, 0.542716, -0.312959 },
    { 2.56586, 0.103505, 0.531154, -0.318571 },
    { 2.61391, 0.0986225, 0.515581, -0.326159 },
    { 2.64438, 0.0957044, 0.506159, -0.33076 },
    { 2.68305, 0.0921812, 0.494664, -0.336389 },
    { 2.72874, 0.0882646, 0.481715, -0.342755 },
    { 2.76977, 0.0849648, 0.470663, -0.348191 },
    { 2.81194, 0.0817523, 0.45977, -0.353574 },
    { 2.84593, 0.0792976, 0.451352, -0.35774 },
    { 2.88577, 0.0765558, 0.441852, -0.362453 },
    { 2.92796, 0.073809, 0.432219, -0.367242 },
    { 2.96781, 0.0713449, 0.423477, -0.3716 },
    { 3.00413, 0.0692087, 0.415816, -0.375427 },
    { 3.04691, 0.0668079, 0.407114, -0.379782 },
    { 3.09877, 0.0640647, 0.397039, -0.38484 },
    { 3.13079, 0.0624505, 0.391045, -0.387856 },
    { 3.17171, 0.0604815, 0.383655, -0.391582 },
    { 3.21626, 0.0584409, 0.375914, -0.39549 },
    { 3.25726, 0.0566501, 0.369043, -0.398971 },
    { 3.29712, 0.0549871, 0.362599, -0.402237 },
    { 3.33226, 0.0535826, 0.357099, -0.405031 },
    { 3.37329, 0.0520081, 0.350876, -0.408198 },
    { 3.4143, 0.0505035, 0.344866, -0.411263 },
    { 3.46116, 0.048864, 0.338246, -0.414645 },
    { 3.49867, 0.0476063, 0.333118, -0.417268 },
    { 3.53384, 0.0464717, 0.328447, -0.419663 },
    { 3.5795, 0.0450584, 0.322571, -0.422682 },
    { 3.61587, 0.0439787, 0.318041, -0.425007 },
    { 3.65919, 0.0427412, 0.312796, -0.427711 },
    { 3.69903, 0.0416485, 0.308121, -0.430123 },
    { 3.73888, 0.0405992, 0.303588, -0.432466 },
    { 3.77403, 0.0397062, 0.299696, -0.434477 },
    { 3.82325, 0.0385004, 0.294396, -0.437225 },
    { 3.86544, 0.0375121, 0.290001, -0.439507 },
    { 3.89706, 0.0367959, 0.286794, -0.441173 },
    { 3.94629, 0.0357233, 0.281942, -0.443697 },
    { 3.98727, 0.0348632, 0.278015, -0.445746 },
    { 4.02128, 0.0341735, 0.274839, -0.447403 },
    { 4.0541, 0.0335265, 0.27184, -0.448969 },
    { 4.10099, 0.0326348, 0.267669, -0.451149 },
    { 4.14433, 0.0318404, 0.263917, -0.453117 },
    { 4.17833, 0.0312393, 0.261055, -0.454616 },
    { 4.22402, 0.0304565, 0.257297, -0.45659 },
    { 4.26033, 0.0298557, 0.254387, -0.458119 },
    { 4.29547, 0.0292887, 0.251623, -0.459576 },
    { 4.34005, 0.0285953, 0.248212, -0.461371 },
    { 4.37517, 0.0280666, 0.24559, -0.462753 },
    { 4.40918, 0.0275668, 0.243097, -0.46407 },
    { 4.46072, 0.0268373, 0.239422, -0.466011 },
    { 4.49116, 0.0264176, 0.237294, -0.467137 },
    { 4.54275, 0.0257309, 0.233779, -0.469 },
    { 4.58257, 0.0252203, 0.231142, -0.470397 },
    { 4.61657, 0.0247939, 0.228926, -0.471573 },
    { 4.66463, 0.0242121, 0.225875, -0.473194 },
    { 4.69512, 0.0238517, 0.223975, -0.474205 },
    { 4.73378, 0.0234093, 0.221622, -0.475455 },
    { 4.77948, 0.0228998, 0.218894, -0.476908 },
    { 4.8205, 0.0224559, 0.216498, -0.478188 },
    { 4.8627, 0.0220144, 0.214094, -0.47947 },
    { 4.89675, 0.0216665, 0.212189, -0.480485 },
    { 4.93655, 0.0212706, 0.210004, -0.481656 },
    { 4.97876, 0.0208632, 0.207739, -0.482867 },
    { 4.99996, 0.0206626, 0.206618, -0.483468 }
};

START_NAMESPACE_DISTRHO

#define NUM_PARAMS 3

#define PARAM_DISTTUBE 0 // 0 to 100 (integer)
#define PARAM_MASTERVOLUME 1 // -inf to 0 (integer/enum) x=arange(0.0, 1.1, 0.1); y=[-inf, -55, -38, -29, -22, -16, -12, -8, -5, -3, 0]
#define PARAM_MASTERMIX 2 // 0 to 100 (integer)

// -----------------------------------------------------------------------------------------------------------

/**
  Plugin that demonstrates the latency API in DPF.
 */
class ReverseCamelPlugin : public Plugin
{
public:
    ReverseCamelPlugin() : Plugin(NUM_PARAMS, 0, 0) // 1st argument: Number of parameters
    {
        sampleRateChanged(getSampleRate());
    }

    ~ReverseCamelPlugin() override
    {
    }

protected:
   /* --------------------------------------------------------------------------------------------------------
    * Information */

   /**
      Get the plugin label.
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const override
    {
        return "reverse-camel";
    }

   /**
      Get an extensive comment/description about the plugin.
    */
    const char* getDescription() const override
    {
        return "Cross-platform Camel Crusher clone.";
    }

   /**
      Get the plugin author/maker.
    */
    const char* getMaker() const override
    {
        return "soerenbnoergaard";
    }

   /**
      Get the plugin homepage.
    */
    const char* getHomePage() const override
    {
        return "https://github.com/soerenbnoergaard/reverse-camel";
    }

   /**
      Get the plugin license name (a single line of text).
      For commercial plugins this should return some short copyright information.
    */
    const char* getLicense() const override
    {
        return "MIT";
    }

   /**
      Get the plugin version, in hexadecimal.
    */
    uint32_t getVersion() const override
    {
        return d_version(0, 0, 0);
    }

   /**
      Get the plugin unique Id.
      This value is used by LADSPA, DSSI and VST plugin formats.
    */
    int64_t getUniqueId() const override
    {
        /* soerenbnoergaard: I just made something up */
        return d_cconst('d', 'K', 'a', 'y');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init */

   /**
      Initialize the parameter @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t index, Parameter& parameter) override
    {

        switch (index) {
        case PARAM_DISTTUBE:
            parameter.hints  = kParameterIsAutomable | kParameterIsInteger;
            parameter.name   = "DistTube";
            parameter.symbol = "DistTube";
            parameter.unit   = "%";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 100.0f;
            break;

        case PARAM_MASTERVOLUME:
            parameter.hints  = kParameterIsAutomable | kParameterIsInteger | kParameterIsLogarithmic;
            parameter.name   = "MasterVolume";
            parameter.symbol = "MasterVolume";
            parameter.unit   = "dB";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = -51.0f;
            parameter.ranges.max = 0.0f;
            break;

        case PARAM_MASTERMIX:
            parameter.hints  = kParameterIsAutomable | kParameterIsInteger;
            parameter.name   = "MasterMix";
            parameter.symbol = "MasterMix";
            parameter.unit   = "%";
            parameter.ranges.def = 100.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 100.0f;
            break;

        default:
            break;
        }

        // Set the default parameter values
        setParameterValue(index, parameter.ranges.def);
    }

   /* --------------------------------------------------------------------------------------------------------
    * Internal data */

   /**
      Get the current value of a parameter.
      The host may call this function from any context, including realtime processing.
    */
    float getParameterValue(uint32_t index) const override
    {
        switch (index) {
        case PARAM_DISTTUBE:
            return param_disttube;
            break;

        case PARAM_MASTERVOLUME:
            return param_mastervolume;
            break;

        case PARAM_MASTERMIX:
            return param_mastermix;
            break;

        default:
            return 0.0;
            break;
        }
    }

   /**
      Change a parameter value.
      The host may call this function from any context, including realtime processing.
      When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
      @note This function will only be called for parameter inputs.
    */
    void setParameterValue(uint32_t index, float value) override
    {
        switch (index) {
        case PARAM_DISTTUBE:
            param_disttube = value;
            break;
        case PARAM_MASTERVOLUME:
            param_mastervolume = value;
            if (value < -50) {
                param_mastervolume_lin = 0.0;
            }
            else {
                param_mastervolume_lin = pow(10.0, value/20.0);
            }
            break;
        case PARAM_MASTERMIX:
            param_mastermix = value;
            param_mastermix_wet = value/100.0;
            param_mastermix_dry = 1.0 - param_mastermix_wet;
            break;
        default:
            break;
        }
    }

   /* --------------------------------------------------------------------------------------------------------
    * Audio/MIDI Processing */

   /**
      Run/process function for plugins without MIDI input.
      @note Some parameters might be null if there are no audio inputs or outputs.
    */
    void run(const float** inputs, float** outputs, uint32_t frames) override
    {
        const float *x;
        float *y;
        float s;

        float p0 = disttube_coeffs[(int)param_disttube][0];
        float p1 = disttube_coeffs[(int)param_disttube][1];
        float p2 = disttube_coeffs[(int)param_disttube][2];
        float p3 = disttube_coeffs[(int)param_disttube][3];

        for (uint32_t ch = 0; ch < 2; ch++) {
            x = inputs[ch];
            y = outputs[ch];

            for (uint32_t n = 0; n < frames; n++) {
                // Apply distortion
                s = p0 * x[n];
                if (s < -0.6) {
                    y[n] = p1*s*s + p2*s + p3;
                }
                else if (s > 0.6) {
                    y[n] = (-p1)*s*s + p2*s + (-p3);
                }
                else {
                    y[n] = s;
                }

                // Mix wet and dry signal
                y[n] = param_mastermix_wet*y[n] + param_mastermix_dry*x[n];

                // Apply master volume
                y[n] *= param_mastervolume_lin;
            }
        }
    }

   /* --------------------------------------------------------------------------------------------------------
    * Callbacks (optional) */

   /**
      Optional callback to inform the plugin about a sample rate change.
      This function will only be called when the plugin is deactivated.
    */
    void sampleRateChanged(double newSampleRate) override
    {
    }

    // -------------------------------------------------------------------------------------------------------

private:

    float param_disttube;
    float param_mastervolume;
    float param_mastervolume_lin;
    float param_mastermix;
    float param_mastermix_wet;
    float param_mastermix_dry;

   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverseCamelPlugin)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new ReverseCamelPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
