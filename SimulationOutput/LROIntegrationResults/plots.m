subplot(2, 2, 1);
plot(t, dx, t, dy, t, dz);
legend('x', 'y', 'z', 'Location', 'northwest');
grid('on');
title('HF');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Position difference [m]');

subplot(2, 2, 2);
plot(t, dvx, t, dvy, t, dvz);
legend('v_x', 'v_y', 'v_z', 'Location', 'northwest');
grid('on');
title('HF');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Velocity difference [m/s]');

subplot(2, 2, 3);
plot(t, dxl, t, dyl, t, dzl);
legend('x', 'y', 'z', 'Location', 'northwest');
grid('on');
title('LF');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Position difference [m]');

subplot(2, 2, 4);
plot(t, dvxl, t, dvyl, t, dvzl);
legend('v_x', 'v_y', 'v_z', 'Location', 'northwest');
grid('on');
title('LF');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Velocity difference [m/s]');