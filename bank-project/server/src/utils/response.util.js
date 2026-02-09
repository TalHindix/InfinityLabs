/**
 * Sends a 200 OK response with the given data as JSON wrapped in standard format.
 */
export const ok = (res, data) => res.status(200).json({ success: true, data });

/**
 * Sends a 201 Created response with the given data as JSON wrapped in standard format.
 */
export const created = (res, data) => res.status(201).json({ success: true, data });

