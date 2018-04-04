subplot(2, 3, 1);
plot(t, first(:, 2), t_2, second(:, 1));
legend('1e-10', '1e-8', 'Location', 'northwest');
grid('on');
title('\Delta x');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Position difference [m]');

subplot(2, 3, 2);
plot(t, first(:, 3), t_2, second(:, 2));
legend('1e-10', '1e-8', 'Location', 'northwest');
grid('on');
title('\Delta y');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Position difference [m]');

subplot(2, 3, 3);
plot(t, first(:, 4), t_2, second(:, 3));
legend('1e-10', '1e-8', 'Location', 'northwest');
grid('on');
title('\Delta z');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Position difference [m]');

subplot(2, 3, 4);
plot(t, first(:, 5), t_2, second(:, 4));
legend('1e-10', '1e-8', 'Location', 'northwest');
grid('on');
title('\Delta V_x');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Velocity difference [m/s]');

subplot(2, 3, 5);
plot(t, first(:, 6), t_2, second(:, 5));
legend('1e-10', '1e-8', 'Location', 'northwest');
grid('on');
title('\Delta V_y');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Velocity difference [m/s]');

subplot(2, 3, 6);
plot(t, first(:, 7), t_2, second(:, 6));
legend('1e-10', '1e-8', 'Location', 'northwest');
grid('on');
title('\Delta V_z');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Velocity difference [m/s]');