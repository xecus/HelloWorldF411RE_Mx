"""
COM5からのBNO055データのサンプリング速度計測スクリプト

期待するデータフォーマット:
  Ax:%.2f Ay:%.2f Az:%.2f Gx:%.2f Gy:%.2f Gz:%.2f\r\n

使い方:
  python measure_sampling_rate.py [--port COM5] [--baud 115200] [--duration 10]
"""

import serial
import time
import argparse
import re
import statistics
from collections import deque


def parse_args():
    parser = argparse.ArgumentParser(description="COMポートからのサンプリング速度計測")
    parser.add_argument("--port", default="COM5", help="シリアルポート (default: COM5)")
    parser.add_argument("--baud", type=int, default=115200, help="ボーレート (default: 115200)")
    parser.add_argument("--duration", type=int, default=10, help="計測時間 [秒] (default: 10)")
    parser.add_argument("--interval", type=float, default=1.0,
                        help="途中経過の表示間隔 [秒] (default: 1.0)")
    return parser.parse_args()


# BNO055出力行のパターン
LINE_PATTERN = re.compile(
    r"Ax:([-\d.]+)\s+Ay:([-\d.]+)\s+Az:([-\d.]+)\s+"
    r"Gx:([-\d.]+)\s+Gy:([-\d.]+)\s+Gz:([-\d.]+)"
)


def main():
    args = parse_args()

    print(f"ポート       : {args.port}")
    print(f"ボーレート   : {args.baud} bps")
    print(f"計測時間     : {args.duration} 秒")
    print("-" * 50)

    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
    except serial.SerialException as e:
        print(f"[ERROR] シリアルポートを開けません: {e}")
        return

    # バッファをフラッシュ
    ser.reset_input_buffer()
    time.sleep(0.1)

    timestamps = []          # 全サンプルの受信時刻
    intervals = deque()      # 連続サンプル間の間隔 [秒]
    parse_errors = 0
    total_lines = 0

    start_time = time.perf_counter()
    last_report_time = start_time
    last_ts = None

    print(f"{'経過[s]':>8}  {'累計':>7}  {'瞬間Hz':>9}  {'平均Hz':>9}  {'エラー':>6}")
    print("-" * 50)

    try:
        while True:
            now = time.perf_counter()
            elapsed = now - start_time

            if elapsed >= args.duration:
                break

            line_bytes = ser.readline()
            if not line_bytes:
                continue

            recv_time = time.perf_counter()
            total_lines += 1

            try:
                line = line_bytes.decode("ascii", errors="replace").strip()
            except Exception:
                parse_errors += 1
                continue

            if not LINE_PATTERN.search(line):
                # 不完全な行やゴミデータはスキップ
                parse_errors += 1
                continue

            timestamps.append(recv_time)

            if last_ts is not None:
                dt = recv_time - last_ts
                intervals.append(dt)
            last_ts = recv_time

            # 途中経過を表示
            if recv_time - last_report_time >= args.interval:
                n = len(timestamps)
                if len(intervals) >= 2:
                    recent = list(intervals)[-50:]          # 直近50サンプルの平均
                    instant_hz = 1.0 / statistics.mean(recent)
                else:
                    instant_hz = 0.0

                total_elapsed = recv_time - start_time
                avg_hz = n / total_elapsed if total_elapsed > 0 else 0.0

                print(f"{total_elapsed:8.2f}  {n:7d}  {instant_hz:9.2f}  {avg_hz:9.2f}  {parse_errors:6d}")
                last_report_time = recv_time

    except KeyboardInterrupt:
        print("\n[割り込み] 計測を中断しました")
    finally:
        ser.close()

    # --- 最終集計 ---
    n = len(timestamps)
    if n < 2:
        print("サンプル数が不足しています。")
        return

    total_elapsed = timestamps[-1] - timestamps[0]
    all_intervals = [timestamps[i+1] - timestamps[i] for i in range(n - 1)]

    mean_interval  = statistics.mean(all_intervals)
    stdev_interval = statistics.stdev(all_intervals) if len(all_intervals) >= 2 else 0.0
    min_interval   = min(all_intervals)
    max_interval   = max(all_intervals)

    mean_hz  = 1.0 / mean_interval
    stdev_hz = stdev_interval / (mean_interval ** 2)  # 誤差伝播
    min_hz   = 1.0 / max_interval
    max_hz   = 1.0 / min_interval

    print()
    print("=" * 50)
    print("  計測結果サマリ")
    print("=" * 50)
    print(f"  有効サンプル数  : {n}")
    print(f"  総受信行数      : {total_lines}")
    print(f"  パースエラー    : {parse_errors}")
    print(f"  計測時間        : {total_elapsed:.3f} 秒")
    print()
    print(f"  サンプリングレート")
    print(f"    平均  : {mean_hz:.2f} Hz")
    print(f"    標準偏差: {stdev_hz:.2f} Hz")
    print(f"    最小  : {min_hz:.2f} Hz")
    print(f"    最大  : {max_hz:.2f} Hz")
    print()
    print(f"  サンプル間隔")
    print(f"    平均  : {mean_interval*1000:.3f} ms")
    print(f"    標準偏差: {stdev_interval*1000:.3f} ms")
    print(f"    最小  : {min_interval*1000:.3f} ms")
    print(f"    最大  : {max_interval*1000:.3f} ms")
    print("=" * 50)


if __name__ == "__main__":
    main()
