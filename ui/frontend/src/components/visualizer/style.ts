import { GraphNode } from '@/visualizer/types'
import _memoize from 'lodash.memoize'

const trimSpaces = (str: string) => str.replace(/\s+/g, ' ')

const _getNodeSvg = ({
  color = '#C9CFDE',
  size,
  selected,
  opacity = 1,
}: {
  color: string
  size?: 'small'
  selected?: boolean
  opacity?: number
}) => {
  const width = size === 'small' ? 25 : 41
  const height = size === 'small' ? 24 : 40
  const cutColor = selected ? '#0E1014' : color

  let svg = ''
  if (size === 'small') {
    svg = `
    <svg width="${width}" height="${height}" style="opacity: ${opacity};" viewBox="0 0 25 24" fill="none" xmlns="http://www.w3.org/2000/svg">
      <path d="M12.5 0L20.9853 3.51472L24.5 12L20.9853 20.4853L12.5 24L4.01472 20.4853L0.5 12L4.01472 3.51472L12.5 0Z" fill="${color}" fill-opacity="0.06"/>
      <path d="M12.5 4.5L17.8033 6.6967L20 12L17.8033 17.3033L12.5 19.5L7.1967 17.3033L5 12L7.1967 6.6967L12.5 4.5Z" fill="${color}"/>
      <path d="M12.5 5.4L17.1669 7.3331L19.1 12L17.1669 16.6669L12.5 18.6L7.83309 16.6669L5.89999 12L7.83309 7.3331L12.5 5.4Z" fill="${cutColor}"/>
      <path d="M12.5 6.6L16.3184 8.18162L17.9 12L16.3184 15.8184L12.5 17.4L8.68163 15.8184L7.10001 12L8.68163 8.18162L12.5 6.6Z" fill="${color}"/>
    </svg>
    `
  } else {
    svg = `
    <svg
      width="${width}"
      height="${height}"
      style="opacity: ${opacity};"
      viewBox="0 0 41 40"
      fill="none"
      xmlns="http://www.w3.org/2000/svg"
    >
      <path
        d="M20.5 0L34.6421 5.85786L40.5 20L34.6421 34.1421L20.5 40L6.35786 34.1421L0.5 20L6.35786 5.85786L20.5 0Z"
        fill="${color}"
        fill-opacity="0.06"
      />
      <path
        d="M20.5 7.5L29.3388 11.1612L33 20L29.3388 28.8388L20.5 32.5L11.6612 28.8388L8 20L11.6612 11.1612L20.5 7.5Z"
        fill="${color}"
      />
      <path
        d="M20.5 9L28.2782 12.2218L31.5 20L28.2782 27.7782L20.5 31L12.7218 27.7782L9.5 20L12.7218 12.2218L20.5 9Z"
        fill="${cutColor}"
      />
      <path
        d="M20.5 11L26.864 13.636L29.5 20L26.864 26.364L20.5 29L14.136 26.364L11.5 20L14.136 13.636L20.5 11Z"
        fill="${color}"
      />
    </svg>`
  }

  const dataUri = `data:image/svg+xml;utf8,${encodeURIComponent(trimSpaces(svg).trim())}`

  return dataUri
}

const getNodeSvg = _memoize(_getNodeSvg)

const octagonPoints = [
  1, 0, 0.707, 0.707, 0, 1, -0.707, 0.707, -1, 0, -0.707, -0.707, 0, -1, 0.707, -0.707,
]

export const getCyStyle = (initialNode?: GraphNode) => {
  const styles = [
    {
      selector: 'core',
      style: {
        'selection-box-color': '#679CFF',
        'selection-box-opacity': '0.3',
      },
    },
    {
      selector: 'node',
      style: {
        'z-index': '10',
        content: 'data(label)',
        'background-color': 'transparent',
        color: 'data(textColor)',
        'font-size': '13px',
        'font-weight': 'normal',
        'text-valign': 'bottom',
        'text-halign': 'right',
        'background-opacity': 1,
        shape: 'polygon',
        'shape-polygon-points': octagonPoints,
        width: 24,
        height: 24,
        'text-opacity': 1,
        'background-clip': 'none',
        'background-image': (ele: any) => getNodeSvg({ color: ele?.data().iconColor }),
        'bounds-expansion': '16px',
        'background-fit': 'none',
        'background-width': 41,
        'background-height': 40,
      },
    },

    {
      selector: 'node[type="neighbor"]',
      style: {
        'font-size': '13px',
        width: 14,
        height: 14,
        'background-width': 24,
        'background-height': 24,
        'background-color': 'transparent',
        'bounds-expansion': '10px',
        'background-clip': 'none',
        'text-opacity': 0.6,
        'background-image': (ele: any) =>
          getNodeSvg({ color: ele?.data().iconColor, size: 'small', opacity: 0.6 }),
      },
    },
    {
      selector: 'node:selected[type="neighbor"]',
      style: {
        'background-image': (ele: any) => getNodeSvg({ color: ele?.data().iconColor }),
      },
    },
    {
      selector: 'edge',
      style: {
        content: 'data(label)',
        'text-rotation': 'autorotate',
        'font-weight': 'normal',
        'text-valign': 'bottom',
        'text-halign': 'right',
        'curve-style': 'bezier',
        'target-arrow-shape': 'triangle',
        'target-arrow-color': 'data(lineColor)',
        'line-color': 'data(lineColor)',
        'arrow-scale': 1,
        'text-opacity': 1,
        color: 'data(textColor)',
        'font-size': 12,
        width: 2,
        'line-style': 'solid',
        'text-wrap': 'wrap',
      },
    },
    {
      selector: 'edge[type="neighbor"]',
      style: {
        width: 1.0,
        'line-opacity': 0.5,
        'text-opacity': 0.5,
      },
    },
    {
      selector: 'node:active',
      style: {
        'overlay-opacity': 0,
      },
    },
    {
      selector: 'node:selected',
      style: {
        color: '#679CFF',
        'background-image': getNodeSvg({ color: '#679CFF' }),
      },
    },
    {
      selector: 'edge:selected',
      style: {
        'line-color': '#679CFF',
        color: '#679CFF',
        'target-arrow-color': '#679CFF',
      },
    },
  ]

  if (initialNode) {
    styles.push({
      selector: `node[id = '${initialNode.data.id}']`,
      style: {
        'background-image': (ele: any) => {
          const defaultMainNodeColor = '#679CFF'
          const defaultColorIconColor = '#C9CFDE'
          const iconColor = ele?.data()?.iconColor

          if (ele?.selected() || iconColor === defaultColorIconColor) {
            return getNodeSvg({ color: defaultMainNodeColor, selected: true })
          }
          return getNodeSvg({ color: iconColor, selected: true })
        },
      },
    })
  }

  return styles
}
