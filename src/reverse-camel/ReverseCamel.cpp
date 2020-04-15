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
    { 0.999997, 2.39905e-08, 1.00112, 12.6909 },
    { 1.07, 5.4457e-05, 3.83459, 34659.9 },
    { 1.094, 4.95223e-05, 3.56081, 31190.2 },
    { 1.138, 4.18465e-05, 3.12811, 25652.6 },
    { 1.166, 3.78765e-05, 2.89958, 22696.7 },
    { 1.228, 3.08537e-05, 2.48551, 17274.3 },
    { 1.268, 2.7307e-05, 2.27049, 14420.2 },
    { 1.308, 2.4323e-05, 2.08581, 11943.6 },
    { 1.32, 2.35312e-05, 2.03608, 11273 },
    { 1.37, 2.05871e-05, 1.84844, 8722.81 },
    { 1.402, 1.89795e-05, 1.74384, 7288.18 },
    { 1.448, 1.69817e-05, 1.61129, 5454.94 },
    { 1.5, 1.50823e-05, 1.48225, 3652.79 },
    { 1.526, 1.42494e-05, 1.42458, 2841.28 },
    { 1.568, 1.30386e-05, 1.33946, 1635.67 },
    { 1.612, 1.19381e-05, 1.26048, 509.83 },
    { 1.658, 1.09238e-05, 1.18625, -557.251 },
    { 1.70401, 1.00337e-05, 1.11976, -1520.09 },
    { 1.74401, 9.33674e-06, 1.06675, -2294.41 },
    { 1.78801, 8.66317e-06, 1.01453, -3059.5 },
    { 1.806, 8.40677e-06, 0.9944, -3356.28 },
    { 1.856, 7.75182e-06, 0.942265, -4127.69 },
    { 1.90201, 7.21343e-06, 0.898556, -4779.07 },
    { 1.92602, 6.95486e-06, 0.877273, -5097.32 },
    { 1.99201, 6.30976e-06, 0.823234, -5910.8 },
    { 2.01399, 6.1152e-06, 0.806643, -6161.85 },
    { 2.062, 5.71943e-06, 0.772458, -6681.21 },
    { 2.086, 5.53583e-06, 0.756378, -6926.57 },
    { 2.13001, 5.22509e-06, 0.728811, -7346.96 },
    { 2.16601, 4.98606e-06, 0.707307, -7678.07 },
    { 2.22402, 4.63698e-06, 0.67536, -8170.8 },
    { 2.242, 4.53414e-06, 0.665825, -8319.51 },
    { 2.30202, 4.21908e-06, 0.636198, -8780.67 },
    { 2.32802, 4.0945e-06, 0.624303, -8965.4 },
    { 2.37802, 3.86633e-06, 0.602236, -9311.94 },
    { 2.404, 3.75765e-06, 0.591583, -9478.2 },
    { 2.46003, 3.53231e-06, 0.569224, -9832.09 },
    { 2.50197, 3.38096e-06, 0.553938, -10072.4 },
    { 2.54398, 3.23465e-06, 0.538983, -10311.1 },
    { 2.57801, 3.12516e-06, 0.527643, -10491.1 },
    { 2.61199, 3.02119e-06, 0.516766, -10664.1 },
    { 2.66, 2.87953e-06, 0.501764, -10906.4 },
    { 2.68404, 2.81286e-06, 0.494621, -11021.5 },
    { 2.73999, 2.66868e-06, 0.478974, -11272.6 },
    { 2.772, 2.59277e-06, 0.470623, -11405.4 },
    { 2.82599, 2.46628e-06, 0.456555, -11635.3 },
    { 2.866, 2.38093e-06, 0.446918, -11790.7 },
    { 2.912, 2.28717e-06, 0.436204, -11965.2 },
    { 2.93001, 2.25299e-06, 0.432259, -12028.4 },
    { 2.96602, 2.18298e-06, 0.424135, -12163.1 },
    { 3.02796, 2.07506e-06, 0.41141, -12369.4 },
    { 3.052, 2.0332e-06, 0.406429, -12452.6 },
    { 3.098, 1.95796e-06, 0.397378, -12602.8 },
    { 3.13201, 1.90663e-06, 0.391128, -12704.6 },
    { 3.172, 1.84748e-06, 0.383862, -12825 },
    { 3.21802, 1.78459e-06, 0.376039, -12952.5 },
    { 3.25201, 1.73847e-06, 0.370251, -13049.4 },
    { 3.29203, 1.68611e-06, 0.363617, -13161.1 },
    { 3.33399, 1.63535e-06, 0.357108, -13268.6 },
    { 3.38602, 1.57418e-06, 0.349178, -13401.8 },
    { 3.41401, 1.54332e-06, 0.345132, -13469 },
    { 3.46996, 1.48465e-06, 0.33736, -13597.7 },
    { 3.51797, 1.43503e-06, 0.330715, -13711.1 },
    { 3.54002, 1.41451e-06, 0.327936, -13756.2 },
    { 3.58003, 1.37633e-06, 0.322735, -13844.5 },
    { 3.61001, 1.34957e-06, 0.319058, -13905.4 },
    { 3.65396, 1.31042e-06, 0.313635, -13998.1 },
    { 3.69404, 1.27664e-06, 0.308905, -14078.1 },
    { 3.72604, 1.25093e-06, 0.305274, -14139 },
    { 3.78808, 1.20168e-06, 0.298254, -14259.9 },
    { 3.82005, 1.17832e-06, 0.294881, -14316.5 },
    { 3.85006, 1.15701e-06, 0.291783, -14368.4 },
    { 3.91002, 1.11485e-06, 0.285599, -14475.1 },
    { 3.952, 1.08707e-06, 0.281477, -14545.4 },
    { 3.97201, 1.07508e-06, 0.27968, -14574.2 },
    { 4.01995, 1.04501e-06, 0.275154, -14651.9 },
    { 4.05796, 1.02244e-06, 0.271725, -14709.9 },
    { 4.08597, 1.00516e-06, 0.269084, -14757.4 },
    { 4.14804, 9.71442e-07, 0.26387, -14843.9 },
    { 4.16209, 9.6322e-07, 0.262594, -14867.2 },
    { 4.21192, 9.36759e-07, 0.258443, -14939 },
    { 4.26197, 9.11311e-07, 0.254414, -15008.3 },
    { 4.30606, 8.89969e-07, 0.250999, -15066.5 },
    { 4.33997, 8.72978e-07, 0.248264, -15116.5 },
    { 4.37392, 8.5786e-07, 0.245804, -15157.5 },
    { 4.42198, 8.36552e-07, 0.242316, -15217.5 },
    { 4.44413, 8.26417e-07, 0.24065, -15247.7 },
    { 4.51195, 7.98374e-07, 0.23598, -15327.7 },
    { 4.53599, 7.88715e-07, 0.23436, -15355.6 },
    { 4.57606, 7.73299e-07, 0.231755, -15399.4 },
    { 4.62202, 7.54746e-07, 0.2286, -15457.7 },
    { 4.65405, 7.43421e-07, 0.226654, -15489.6 },
    { 4.69803, 7.27904e-07, 0.223971, -15535.2 },
    { 4.75203, 7.08639e-07, 0.220614, -15595.3 },
    { 4.7821, 6.98527e-07, 0.218836, -15626.2 },
    { 4.82206, 6.85804e-07, 0.216582, -15664 },
    { 4.86398, 6.71628e-07, 0.214059, -15711 },
    { 4.8839, 6.66218e-07, 0.213083, -15725.1 },
    { 4.92609, 6.52565e-07, 0.210626, -15770.8 },
    { 4.97401, 6.38672e-07, 0.2081, -15813.8 },
    { 4.99999, 6.30729e-07, 0.206647, -15841 }
};

START_NAMESPACE_DISTRHO

#define PARAM_DISTTUBE 0

// -----------------------------------------------------------------------------------------------------------

/**
  Plugin that demonstrates the latency API in DPF.
 */
class ReverseCamelPlugin : public Plugin
{
public:
    ReverseCamelPlugin() : Plugin(1, 0, 0) // 1st argument: Number of parameters
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
        return d_version(0, 0, 1);
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

        default:
            break;
        }
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
            return disttube;
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
            disttube = value;
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

        float p0 = disttube_coeffs[(int)disttube][0];
        float p1 = disttube_coeffs[(int)disttube][1];
        float p2 = disttube_coeffs[(int)disttube][2];
        float p3 = disttube_coeffs[(int)disttube][3];

        for (uint32_t ch = 0; ch < 2; ch++) {
            x = inputs[ch];
            y = outputs[ch];

            for (uint32_t n = 0; n < frames; n++) {
                s = p0 * x[n] * 32768;

                if (s < -19400) {
                    y[n] = p1*s*s + p2*s + p3;
                }
                else if (s > 19400) {
                    y[n] = (-p1)*s*s + p2*s + (-p3);
                }
                else {
                    y[n] = s;
                }

                y[n] /= 32768;
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

    float disttube;

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
