i = 1;

while i <= 6 
    subplot(2, 3, i);
    plot(t, KeplerElements_HF(:, i+1));
    i = i + 1;
end

i = 1;
figure;

while i <= 6 
    subplot(2, 3, i);
    plot(t, KeplerElements_LF(:, i+1));
    i = i + 1;
end