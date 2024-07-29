function ssim_value = image_ssim(file1, file2)
    % Read the 16-bit binary images
    [img1, width1, height1] = read_16bit_binary_image(file1);
    [img2, width2, height2] = read_16bit_binary_image(file2);

    % Ensure the images have the same dimensions
    if width1 ~= width2 || height1 ~= height2
        error('Images must have the same dimensions');
    end

    % Compute SSIM
    ssim_value = compute_ssim(img1, img2);

    % Display the SSIM value
    fprintf('SSIM: %f\n', ssim_value);
end

function [image, width, height] = read_16bit_binary_image(filename)
    % Open the file
    fileID = fopen(filename, 'rb');
    if fileID == -1
        error('Cannot open or find the file: %s', filename);
    end

    % Read the width and height
    width = fread(fileID, 1, 'int32');
    height = fread(fileID, 1, 'int32');

    if width <= 0 || height <= 0
        error('Invalid width or height in file: %s', filename);
    end

    % Read the image data
    size = width * height;
    image = fread(fileID, size, 'uint16=>double');
    image = reshape(image, [height, width]);

    % Close the file
    fclose(fileID);
end

function ssim_value = compute_ssim(img1, img2)
    % Parameters for SSIM
    K1 = 0.01;
    K2 = 0.03;
    L = 65535; % Since pixel values are in the range [0, 65535]
    C1 = (K1 * L) ^ 2;
    C2 = (K2 * L) ^ 2;

    % Convert images to double
    img1 = double(img1);
    img2 = double(img2);

    % Compute means
    mu1 = mean(img1(:));
    mu2 = mean(img2(:));

    % Compute variances
    sigma1_sq = var(img1(:));
    sigma2_sq = var(img2(:));

    % Compute covariance
    sigma12 = cov(img1(:), img2(:));
    sigma12 = sigma12(1, 2);

    % Compute SSIM
    numerator = (2 * mu1 * mu2 + C1) * (2 * sigma12 + C2);
    denominator = (mu1^2 + mu2^2 + C1) * (sigma1_sq + sigma2_sq + C2);
    ssim_value = numerator / denominator;
end
