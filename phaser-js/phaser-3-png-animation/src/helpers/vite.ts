export function logPrefixFilename(fileUrl: string): string {
    // Remove protocol (file://) and get the path
    const path = fileUrl.replace(/^file:\/\//, '');
    // Remove query parameters
    const cleanPath = path.replace(/\?.*$/, '');
    // Get last two segments (parent folder + filename)
    const parts = cleanPath.split('/');
    return parts.slice(-2).join('/');
}
