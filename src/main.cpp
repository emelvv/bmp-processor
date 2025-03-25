#include <iostream>

#include "filters/crop.h"
#include "filters/edge_detection.h"
#include "filters/gaussian_blur.h"
#include "filters/grayscale.h"
#include "filters/motion_blur.h"
#include "filters/negative.h"
#include "filters/sharpening.h"
#include "utility/args.h"
#include "utility/image.h"

int main(int argc, const char* argv[])
{
    try
    {
        Args args(argc, argv);

        std::cout << "Input file: " << args.GetInputFile() << "\n";
        std::cout << "Output file: " << args.GetOutputFile() << "\n";

        const auto& filters = args.GetFilters();

        Image img(args.GetInputFile());

        std::cout << "Apllying filters..." << "\n";

        for (const auto& filter : filters)
        {
            if (filter.GetName() == "crop")
            {
                Crop crop(filter.GetParameters());
                crop.Apply(img);
            }
            else if (filter.GetName() == "gs")
            {
                Grayscale gs(filter.GetParameters());
                gs.Apply(img);
            }
            else if (filter.GetName() == "neg")
            {
                Negative neg(filter.GetParameters());
                neg.Apply(img);
            }
            else if (filter.GetName() == "sharp")
            {
                Sharpening sharp(filter.GetParameters());
                sharp.Apply(img);
            }
            else if (filter.GetName() == "edge")
            {
                EdgeDetection edge(filter.GetParameters());
                edge.Apply(img);
            }
            else if (filter.GetName() == "blur")
            {
                GaussianBlur blur(filter.GetParameters());
                blur.Apply(img);
            }
            else if (filter.GetName() == "motion")
            {
                MotionBlur motion(filter.GetParameters());
                motion.Apply(img);
            }
        }

        img.ApplyChanges();
        img.Save(args.GetOutputFile());

        std::cout << "Done successfully." << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
