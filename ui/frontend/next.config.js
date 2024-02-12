/** @type {import('next').NextConfig} */
console.log('== NEXT config ==')
console.log(`  - NEXT_DEV_MODE: ${process.env.NEXT_DEV_MODE ? 'true' : 'false'}`)

const devConfig = process.env.NEXT_DEV_MODE
  ? {
      async rewrites() {
        return [
          {
            source: '/api/:path*',
            destination: 'http://localhost:5000/api/:path*',
          },
        ]
      },
    }
  : {
      output: 'export',
    }

const nextConfig = {
  reactStrictMode: false,
  distDir: 'site',
  ...devConfig,
  webpack(config) {
    // Grab the existing rule that handles SVG imports
    const fileLoaderRule = config.module.rules.find((rule) => rule.test?.test?.('.svg'))

    config.module.rules.push(
      // Reapply the existing rule, but only for svg imports ending in ?url
      {
        ...fileLoaderRule,
        test: /\.svg$/i,
        resourceQuery: /url/, // *.svg?url
      },
      // Convert all other *.svg imports to React components
      {
        test: /\.svg$/i,
        issuer: fileLoaderRule.issuer,
        resourceQuery: { not: [...fileLoaderRule.resourceQuery.not, /url/] }, // exclude if *.svg?url
        use: ['@svgr/webpack'],
      }
    )

    // Modify the file loader rule to ignore *.svg, since we have it handled now.
    fileLoaderRule.exclude = /\.svg$/i

    return config
  },
  typescript: {
    // TODO remove this when src/visualizer is fully converted to TypeScript
    ignoreBuildErrors: true,
  },
}

module.exports = nextConfig
