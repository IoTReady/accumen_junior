try:
    from app import start
    start()
except Exception as e:
    print("E: Exception loading app", str(e))
    try:
        from app_previous import start
        start()
    except Exception as e:
        print("E: Exception loading app_previous", str(e))
        from app_factory import start
        start()
