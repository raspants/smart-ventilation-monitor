import time
import threading

from db import mark_offline_devices

def offline_check_loop(timeout_seconds=30, check_interval=10):
    while True:
        try:
            count = mark_offline_devices(timeout_seconds)

            if count > 0:
                print(
                    f"Marked {count} device(s) as OFFLINE",
                    flush=True
                )

        except Exception as error:
            print(
                f"Offline check failed: {error}",
                flush=True
            )

        time.sleep(check_interval)

def start_offline_monitor():
    thread = threading.Thread(
        target=offline_check_loop,
        daemon=True
    )
    thread.start()

    return thread