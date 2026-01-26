export const ok = (res, data) =>
  res.status(200).json(data);

export const created = (res, data) =>
  res.status(201).json(data);

export const badRequest = (res) =>
  res.status(400).json(null);

export const unauthorized = (res) =>
  res.status(401).json(null);

export const notFound = (res) =>
  res.status(404).json(null);

export const serverError = (res) =>
  res.status(500).json(null);
