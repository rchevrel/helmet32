String getStringPaddedForTime(int value) {
  if (value == -1) {
    return "--";
  }
  String valueText = String(value);
  if (value < 10) {
    valueText = "0" + valueText;
  }
  return valueText;
}

String intAsText(int value) {
  if (value == -1) {
    return "-";
  }
  return String(value);
}
