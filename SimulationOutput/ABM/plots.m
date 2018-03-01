subplot(1, 2, 1);
plot(t, first(:, 2), t_2, second(:, 1));
legend('1e-10', '1e-8', 'Location', 'northwest');
grid('on');
title('\Delta x');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Position difference [m]');

subplot(1, 2, 2);
plot(t, first(:, 5), t_2, second(:, 4));
legend('1e-10', '1e-8', 'Location', 'northwest');
grid('on');
title('\Delta V_x');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Velocity difference [m/s]');