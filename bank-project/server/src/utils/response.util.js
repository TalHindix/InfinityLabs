/**
 * Sends a 200 OK response with the given data as JSON.
 */
export const ok = (res, data) => res.status(200).json(data);

/**
 * Sends a 201 Created response with the given data as JSON.
 */
export const created = (res, data) => res.status(201).json(data);

